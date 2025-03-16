//
//  WidgetExtensionBundle.swift
//  WidgetExtension
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import WidgetKit
import SwiftUI
import AppIntents
import Shared

@main
struct WidgetExtension: Widget {
    let kind: String = "WidgetExtension"
    
    init() {
        AppDependencyManager.shared.add(dependency: DeviceManager.shared)
    }

    var body: some WidgetConfiguration {
        AppIntentConfiguration(
            kind: kind,
            intent: SelectDeviceIntent.self,
            provider: AirQualityProvider()) { entry in
                AirQualityWidgetView(entry: entry)
                    .containerBackground(.fill.tertiary, for: .widget)
        }
        .configurationDisplayName("AirQuality")
        .description("Get a glance of you current air quality.")
        .supportedFamilies([
            .systemMedium
        ])
    }
}

#Preview(as: .systemMedium) {
    WidgetExtension()
} timeline: {
    AirQualityEntry(score: 100, temperature: .init(value: 0, unit: .celsius), humidity: 0, pressure: .init(value: 0, unit: .hectopascals), eCO2: 0, date: Date())
}
