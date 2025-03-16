//
//  Configuration.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 04/03/2025.
//

import Foundation

public enum Configuration {
    enum Error: Swift.Error {
        case missingKey, invalidValue
    }
    
    enum Key: String {
        case serverUrl = "SERVER_URL"
    }

    static func value<T>(for key: Key) throws -> T where T: LosslessStringConvertible {
        guard let object = Bundle.main.object(forInfoDictionaryKey: key.rawValue) else {
            throw Error.missingKey
        }

        switch object {
        case let value as T:
            return value
        case let string as String:
            guard let value = T(string) else { fallthrough }
            return value
        default:
            throw Error.invalidValue
        }
    }
}
