//
//  PairingNetworkService.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import Combine

class CloudNetworkService {
    private let request: ApiRequest

    init(serialNumber: UUID) {
        do {
            let serverUrl: String = try Configuration.value(for: .serverUrl)
            
            self.request = ApiRequest(baseUrl: "https://\(serverUrl)", headers: [
                "Station-Number": serialNumber.uuidString
            ])
        } catch {
            fatalError()
        }
    }
    
    func fetchDevice() -> AnyPublisher<DeviceDTO, ApiError> {
        let publisher: AnyPublisher<CloudResponseDTO<DeviceDTO>, ApiError> = self.request.request(endpoint: CloudEndpoint.devices)
        
        return publisher
            .map(\.data)
            .eraseToAnyPublisher()
    }
    
    func fetchMeasurement() -> AnyPublisher<SensorsDTO, ApiError> {
        let publisher: AnyPublisher<CloudResponseDTO<SensorsDTO>, ApiError> = self.request.request(endpoint: CloudEndpoint.measurements)
        
        return publisher
            .map(\.data)
            .eraseToAnyPublisher()
    }
}
