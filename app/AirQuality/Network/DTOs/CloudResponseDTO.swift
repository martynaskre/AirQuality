//
//  CloudResponseDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 01/03/2025.
//

struct CloudResponseDTO<T: Decodable>: Decodable {
    let data: T
}
