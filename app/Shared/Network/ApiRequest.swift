//
//  ApiRequest.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import Combine
import OSLog

public class ApiRequest {
    private let baseUrl: String
    private let session: URLSession
    private let decoder: JSONDecoder
    
    init(baseUrl: String, headers: [String: String] = [:]) {
        self.baseUrl = baseUrl
        
        self.decoder = JSONDecoder()
        self.decoder.keyDecodingStrategy = .convertFromSnakeCase
        self.decoder.dateDecodingStrategy = .iso8601
        
        let configuration = URLSessionConfiguration.default
        configuration.urlCache = .shared
        configuration.requestCachePolicy = .reloadRevalidatingCacheData
        configuration.timeoutIntervalForRequest = 60
        configuration.timeoutIntervalForResource = 120
        configuration.httpAdditionalHeaders = [
            "Accept": "application/json",
        ].merging(with: headers)
        
        self.session = URLSession(configuration: configuration)
    }
    
    private func makeURL(endpoint: Endpoint) -> URL {
        return URL(string: "\(baseUrl)/\(endpoint.path())")!
    }
    
    static private func makeRequest(url: URL, httpMethod: String = "GET", params: [(String, String)]? = nil, formData: Encodable? = nil) -> URLRequest {
        var request = URLRequest(url: url)
        request.httpMethod = httpMethod

        if let params = params {
            var components = URLComponents(url: url, resolvingAgainstBaseURL: false)

            var queryItems: [URLQueryItem] = []

            for (key, value) in params {
                let queryItem = URLQueryItem(name: key, value: value)
                queryItems.append(queryItem)
            }

            components?.queryItems = queryItems

            if let updatedURL = components?.url {
                request.url = updatedURL
            }
        }
        
        if let formData = formData {
            do {
                request.setValue("application/json", forHTTPHeaderField: "Content-Type")
                request.httpBody = try JSONEncoder().encode(formData)
            } catch {
                Logger.networkLogging.debug("[Network] An error occurred while preparing form data: \(error)")
            }
        }

        return request
    }
    
    func request<T: Decodable>(url: URL, httpMethod: String = "GET", params: [(String, String)]? = nil, formData: Encodable? = nil) -> AnyPublisher<T, ApiError> {
        let request = Self.makeRequest(url: url, httpMethod: httpMethod, params: params, formData: formData)

        if let url = request.url?.debugDescription {
            Logger.networkLogging.debug("[Network] Request to: \(url)")
        }
                
        return session.dataTaskPublisher(for: request)
            .tryMap { data, response in
                return try ApiError.processResponse(data: data, response: response)
            }
            .decode(type: T.self, decoder: decoder)
            .mapError { error in
                if let apiError = error as? ApiError {
                    return apiError
                } else {
                    Logger.networkLogging.error("\(error)")

                    return ApiError.unknown
                }
            }
            .eraseToAnyPublisher()
    }
    
    func request<T: Decodable>(endpoint: Endpoint, httpMethod: String = "GET", params: [(String, String)]? = nil, formData: Encodable? = nil) -> AnyPublisher<T, ApiError> {
        let url = makeURL(endpoint: endpoint)
        
        return request(url: url, httpMethod: httpMethod, params: params, formData: formData)
    }
    
    func request(url: URL, httpMethod: String = "GET", params: [(String, String)]? = nil, formData: Encodable? = nil) -> AnyPublisher<Void, ApiError> {
        let request = Self.makeRequest(url: url, httpMethod: httpMethod, params: params, formData: formData)

        if let url = request.url?.debugDescription {
            Logger.networkLogging.debug("[Network] Request to: \(url)")
        }
        
        return session.dataTaskPublisher(for: request)
            .tryMap { data, response in
                return try ApiError.processResponse(data: data, response: response)
            }
            .map { _ in () }
            .mapError { error in
                if let apiError = error as? ApiError {
                    return apiError
                } else {
                    Logger.networkLogging.error("[Network] A parsing error occurred: \(error)")

                    return ApiError.unknown
                }
            }
            .eraseToAnyPublisher()
    }
    
    func request(endpoint: Endpoint, httpMethod: String = "GET", params: [(String, String)]? = nil, formData: Encodable? = nil) -> AnyPublisher<Void, ApiError> {
        let url = makeURL(endpoint: endpoint)
        
        return request(url: url, httpMethod: httpMethod, params: params, formData: formData)
    }
}
