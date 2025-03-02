//
//  DevicesView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import SwiftUI
import SwiftData

struct DevicesView: View {
    @Environment(UiState.self) var uiState
    @Environment(AccessoryManager.self) var accessoryManager
    @Environment(\.modelContext) private var modelContext
    
    @Query(sort: \Device.name) private var devices: [Device]
    
    var body: some View {
        VStack {
            if devices.isEmpty {
                VStack(spacing: 10) {
                    Image(systemName: "x.circle.fill")
                        .resizable()
                        .foregroundColor(.secondary)
                        .frame(width: 55, height: 55)
                    Text("No Devices")
                        .font(.largeTitle)
                        .bold()
                        .foregroundStyle(.primary)
                    Text("To add a compatible device, click the find devices icon in the toolbar.")
                        .multilineTextAlignment(.center)
                        .foregroundColor(.secondary)
                        .frame(width: 300)
                }
            } else {
                List {
                    ForEach(devices, id: \.self) { device in
                        NavigationLink(value: UiState.ApplicationRoute.device(device)) {
                            HStack {
                                Text(device.name)
                            }
                        }
                    }
                    .onDelete(perform: delete)
                }
            }

        }
        .onChange(of: accessoryManager.pairingSession) { _, pairingSession in
            if let pairingSession = pairingSession {
                uiState.navigationPath.append(.pairing(pairingSession))
                
                accessoryManager.reset()
            }
        }
        .toolbar {
            if accessoryManager.activated {
                ToolbarItem(placement: .automatic) {
                    Button {
                        accessoryManager.presentPicker()
                    } label: {
                        Image(systemName: "plus.circle.fill")
                    }
                }
            }
        }
        .navigationTitle("My Devices")
        .navigationBarTitleDisplayMode(.inline)
    }
    
    private func delete(at offsets: IndexSet) {
        for i in offsets {
            let device = devices[i]
            
            if let accessory = accessoryManager.pairedAccessories.first(where: { $0.bluetoothIdentifier == device.accessoryId }) {
                accessoryManager.removeAccessory(accessory)
            }
            
            modelContext.delete(device)
        }
    }
}

#Preview {
    DevicesView()
        .environment(UiState())
        .environment(AccessoryManager())
}
