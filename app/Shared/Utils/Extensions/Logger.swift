//
//  Logger.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import OSLog

public extension Logger {
    static let networkLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Network")
    static let dataManagerLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Data Manager")
    static let entityQueryLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Entity Query")
    static let widgetLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Widget")
}
