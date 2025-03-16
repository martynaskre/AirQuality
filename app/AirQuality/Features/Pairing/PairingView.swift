//
//  PairingView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import SwiftUI
import AccessorySetupKit
import Shared

struct PairingView: View {
    @Environment(UiState.self) var uiState
    @Environment(AccessoryManager.self) var accessoryManager

    @State private var pairingSession: PairingSession
    
    @State private var ssid: String = ""
    @State private var password: String = ""
    
    init(_ pairingSession: PairingSession) {
        self._pairingSession = .init(initialValue: pairingSession)
    }

    var body: some View {
        Group {
            if pairingSession.status == .error {
                errorBody
            } else if pairingSession.status == .connected {
                pairingBody
            } else if pairingSession.status == .paired {
                pairedBody
            } else {
                connectingBody
            }
        }
        .onChange(of: pairingSession.status) { _, status in
            if status == .error {
                pairingSession.disconnect()
                accessoryManager.terminatePairingSession(pairingSession)
            } else if status == .paired {
                if let accessoryId = pairingSession.accessory.bluetoothIdentifier,
                   let serialNumber = pairingSession.serialNumber {
                 
                    let newDevice = Device(
                        name: pairingSession.accessory.displayName,
                        accessoryId: accessoryId,
                        serialNumber: serialNumber
                    )
                    
                    DeviceManager.shared.create(newDevice)
                }
            }
        }
        .navigationTitle("Pairing")
        .navigationBarTitleDisplayMode(.inline)
        .navigationBarBackButtonHidden()
    }
    
    private var connectingBody: some View {
        VStack(spacing: 20) {
            ProgressView()
                .controlSize(.extraLarge)
            Text("Connecting...")
        }
    }
    
    private var errorBody: some View {
        VStack(spacing: 20) {
            Image(systemName: "exclamationmark.triangle.fill")
                .resizable()
                .scaledToFit()
                .frame(width: 150, height: 150)
            Text("An error occurred. Please try again.")
                .frame(width: 250)
                .multilineTextAlignment(.center)
            Button("Go Back") {
                uiState.navigationPath = []
            }
        }
    }
    
    private var pairingBody: some View {
        VStack(spacing: 20) {
            VStack {
                Text("Setup your \(pairingSession.accessory.displayName)")
                    .font(.title)
            }
            Text("Enter your home network credentials to connect your \(pairingSession.accessory.displayName) to the cloud.")
                .frame(width: 300)
                .multilineTextAlignment(.center)
            Form {
                VStack(alignment: .leading) {
                    TextField("WiFi Network", text: $ssid)
                        .disabled(pairingSession.connectionInProgress)
                    if pairingSession.connectionSuccessful == false {
                        Text("Invalid WiFi network")
                            .foregroundStyle(.red)
                    }
                }
                VStack(alignment: .leading) {
                    SecureField("Passphrase", text: $password)
                        .textContentType(.password)
                        .disabled(pairingSession.connectionInProgress)
                    if pairingSession.connectionSuccessful == false {
                        Text("Invalid passphrase")
                            .foregroundStyle(.red)
                    }
                }
                HStack {
                    Button(role: .destructive) {
                        pairingSession.disconnect()
                        accessoryManager.terminatePairingSession(pairingSession)
                        uiState.navigationPath = []
                    } label: {
                        Text("Cancel")
                    }
                    .buttonStyle(.plain)
                    .foregroundStyle(.red)
                    .disabled(pairingSession.connectionInProgress)
                    Spacer()
                    ProgressView()
                        .controlSize(.mini)
                        .opacity(pairingSession.connectionInProgress ? 1 : 0)
                    Button("Continue") {
                        pairingSession.setNetworkCredentials(ssid: ssid, password: password)
                    }
                    .buttonStyle(.plain)
                    .foregroundStyle(.blue)
                    .disabled(pairingSession.connectionInProgress || ssid == "" || password == "")
                }
            }
        }
        .padding(.top)
    }
    
    private var pairedBody: some View {
        VStack(spacing: 20) {
            Image(systemName: "checkmark.seal.fill")
                .resizable()
                .scaledToFit()
                .frame(width: 150, height: 150)
            Text("Device paired successfully")
                .frame(width: 250)
                .multilineTextAlignment(.center)
            Button("Go Back") {
                uiState.navigationPath = []
            }
        }
    }
}

#Preview {
    let pairingSession = PairingSession(ASAccessory())
    
    return PairingView(pairingSession)
        .environment(UiState())
        .environment(AccessoryManager())
}
