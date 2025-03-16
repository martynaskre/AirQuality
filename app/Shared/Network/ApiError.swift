//
//  ApiError.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import OSLog

public enum ApiError: Error, Equatable {
    case unknown
    
    static func processResponse(data: Data, response: URLResponse) throws -> Data {
        guard let httpResponse = response as? HTTPURLResponse else {
            throw Self.unknown
        }
        
        if 200 ... 299 ~= httpResponse.statusCode {
            return data
        }
        
        print(String(data: data, encoding: .utf8))
                
        throw Self.unknown
    }
}
