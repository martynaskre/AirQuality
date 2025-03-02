//
//  AirQualityApp.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 30/01/2025.
//

import SwiftUI

@main
struct AirQualityApp: App {
    @State var uiState = UiState()
    @State var pairingManager = AccessoryManager()
    
    var body: some Scene {
        WindowGroup {
            NavigationStack(path: $uiState.navigationPath) {
                DevicesView()
                    .navigationDestination(for: UiState.ApplicationRoute.self) { route in
                        switch route {
                        case .pairing(let pairingSession):
                            PairingView(pairingSession)
                        case .device(let device):
                            DeviceView(device)
                        }
                    }
            }
            .navigationViewStyle(.stack)
            .modelContainer(for: [Device.self])
        }
        .environment(uiState)
        .environment(pairingManager)
    }
}
