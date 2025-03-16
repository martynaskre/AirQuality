//
//  WigetConfigurationIntent.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import WidgetKit
import AppIntents

struct SelectDeviceIntent: WidgetConfigurationIntent {
    static var title: LocalizedStringResource = "Select Device"
    static var description = IntentDescription("Select the device to display information for.")

    @Parameter(title: "Device")
    var device: DeviceEntity?

    init(device: DeviceEntity) {
        self.device = device
    }
    
    init() {}
}
