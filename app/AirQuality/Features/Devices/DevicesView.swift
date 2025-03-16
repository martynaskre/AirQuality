//
//  DevicesView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import SwiftUI
import Shared
import Combine

struct DevicesView: View {
    @Environment(UiState.self) var uiState
    @Environment(AccessoryManager.self) var accessoryManager
    
    @State private var devices: [Device] = []
    @State private var deviceData: [UUID: DeviceDTO] = [:]
    @State private var isLoading = false
    
    @State private var cancellables = Set<AnyCancellable>()
    
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
                            HStack(spacing: 15) {
                                if let data = deviceData[device.id] {
                                    VStack {
                                        data.status.image
                                            .font(.title2)
                                    }
                                    .frame(width: 45, height: 45)
                                    .background(.gray.opacity(0.15))
                                    .background(data.status.color)
                                    .clipShape(RoundedRectangle(cornerRadius: 5))
                                } else {
                                    VStack {
                                        ProgressView()
                                    }
                                    .frame(width: 45, height: 45)
                                    .background(.gray.opacity(0.15))
                                    .clipShape(RoundedRectangle(cornerRadius: 5))
                                }
                                VStack(alignment: .leading) {
                                    Text(device.name)
                                        .font(.title2)
                                    if let data = deviceData[device.id] {
                                        Text("Device is \(data.status.rawValue)")
                                            .font(.footnote)
                                    } else {
                                        Text("Loading status...")
                                            .font(.footnote)
                                    }
                                }
                                Spacer()
                            }
                        }
                    }
                    .onDelete(perform: delete)
                }
                .refreshable {
                    self.fetchAllDevices()
                }
            }

        }
        .onAppear {
            self.fetchAllDevices()
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
            DeviceManager.shared.delete(for: [devices[i].id], accessoryManager: accessoryManager)
        }
        
        self.fetchAllDevices()
    }
    
    private func fetchAllDevices() {
        guard !isLoading else { return }
        isLoading = true
        
        devices = DeviceManager.shared.get()
        
        let publishers = devices.map { device in
            device.getCloudService().fetchDevice()
                .map { (device.id, $0) }
                .catch { _ in Empty<(UUID, DeviceDTO), Never>() }
                .eraseToAnyPublisher()
        }
        
        Publishers.MergeMany(publishers)
            .collect()
            .receive(on: DispatchQueue.main)
            .sink { completion in
                if case .failure(let error) = completion {
                    print("Error fetching devices: \(error)")
                }
                isLoading = false
            } receiveValue: { results in
                deviceData = Dictionary(uniqueKeysWithValues: results)
            }
            .store(in: &cancellables)
    }
}

#Preview {
    DevicesView()
        .environment(UiState())
        .environment(AccessoryManager())
}
