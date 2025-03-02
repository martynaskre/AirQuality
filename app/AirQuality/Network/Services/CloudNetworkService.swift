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
        self.request = ApiRequest(baseUrl: "http://192.168.1.163:8080/api", headers: [
            "Station-Number": serialNumber.uuidString
        ])
    }
    
    func fetchMeasurement() -> AnyPublisher<SensorsDTO, ApiError> {
        let publisher: AnyPublisher<CloudResponseDTO<SensorsDTO>, ApiError> = self.request.request(endpoint: CloudEndpoint.measurements)
        
        return publisher
            .map(\.data)
            .eraseToAnyPublisher()
    }
}
