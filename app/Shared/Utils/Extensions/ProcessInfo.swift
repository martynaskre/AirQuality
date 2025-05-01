//
//  ProcessInfo.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import Foundation

public extension ProcessInfo {
    var isPreview: Bool {
        environment["XCODE_RUNNING_FOR_PREVIEWS"] == "1"
    }
}
