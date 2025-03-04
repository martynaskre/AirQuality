//
//  DeviceViewModel.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 16/02/2025.
//

import SwiftUI
import Network
import Combine

extension DeviceView {
    @Observable
    class ViewModel {
        let device: Device
        
        private let cloudService: CloudNetworkService
        
        private(set) var sensorData: SensorsDTO? = nil
        
        private var webSocket: WebSocket?
        private(set) var liveModeAvailable: Bool?
        private(set) var liveModeProgress: Bool = false
        var liveModeActive: Bool {
            return webSocket != nil
        }
        
        private var measurementsDisposable: AnyCancellable? = nil
        
        init(_ device: Device) {
            self.device = device
            self.cloudService = device.getCloudService()
            
            self.fetchSensors()
            self.checkLocalNetworkAvailability()
        }
        
        private func checkLocalNetworkAvailability(timeout: TimeInterval = 5) {
            let host = NWEndpoint.Host(self.device.localUrl)
            let connection = NWConnection(host: host, port: 80, using: .tcp)
            
            connection.stateUpdateHandler = { [weak self] state in
                switch state {
                case .ready:
                    self?.liveModeAvailable = true
                    
                    connection.cancel()
                default:
                    break
                }
            }

            connection.start(queue: DispatchQueue.global())
            
            DispatchQueue.global().asyncAfter(deadline: .now() + timeout) { [weak self] in
                if connection.state != .cancelled {
                    connection.cancel()
                }
                
                if self?.liveModeAvailable == nil {
                    self?.liveModeAvailable = false
                }
            }
        }
        
        func fetchSensors() {
            self.measurementsDisposable = self.cloudService
                .fetchMeasurement()
                .receive(on: DispatchQueue.main)
                .sink { [weak self] _ in
                    self?.measurementsDisposable = nil
                } receiveValue: { [weak self] sensorData in
                    self?.sensorData = sensorData
                }
        }
        
        func toggleLiveMode() {
            if webSocket == nil {
                self.liveModeProgress = true
                
                self.webSocket = WebSocket(self.device.webSocketUrl)
                self.webSocket?.delegate = self
                self.webSocket?.connect()
            } else {
                self.liveModeProgress = true
                
                self.webSocket?.disconnect()
                self.webSocket?.delegate = nil
                self.webSocket = nil
                
                self.liveModeProgress = false
                
                self.fetchSensors()
            }
        }
    }
}

/// MARK: WebSocketDelegate
extension DeviceView.ViewModel: WebSocketDelegate {
    func webSocket(connectionStatus: Bool) {
        if self.webSocket != nil {
            self.liveModeProgress = false
        }
    }
    
    func webSocket(didReceiveMessage message: Data) {        
        do {
            let sensorData = try JSONDecoder().decode(SensorsDTO.self, from: message)
            
            self.sensorData = sensorData
        } catch {}
    }
}
