//
//  Array.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 01/03/2025.
//

public extension Dictionary where Key == String, Value == String {
    func merging(with other: [String: String]) -> [String: String] {
        var merged = self
        
        other.forEach { key, value in
            merged[key] = value
        }
        
        return merged
    }
}
