//
//  Logger.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import OSLog

extension Logger {
    static let networkLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Network")
}
