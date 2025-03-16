//
//  Logger.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation
import OSLog

extension Logger {
    public static let networkLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Network")
    public static let dataManagerLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Data Manager")
    public static let entityQueryLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Entity Query")
    public static let widgetLogging = Logger(subsystem: Bundle.main.bundleIdentifier!, category: "Widget")
}
