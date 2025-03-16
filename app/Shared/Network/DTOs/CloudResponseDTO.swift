//
//  CloudResponseDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 01/03/2025.
//

public struct CloudResponseDTO<T: Decodable>: Decodable {
    public let data: T
}
