//
//  AirQualityProvider.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import WidgetKit
import Shared
import Combine
import AppIntents
import OSLog

struct AirQualityProvider: AppIntentTimelineProvider {
    @AppDependency
    private var deviceManager: DeviceManager
    
    func placeholder(in context: Context) -> AirQualityEntry {
        return AirQualityEntry(
            score: 100,
            temperature: .init(value: 20, unit: .celsius),
            humidity: 45,
            pressure: .init(value: 10, unit: .hectopascals),
            eCO2: 400,
            date: Date()
        )
    }
    
    func snapshot(for configuration: SelectDeviceIntent, in context: Context) async -> AirQualityEntry {
        return placeholder(in: context)
    }
    
    func timeline(for configuration: SelectDeviceIntent, in context: Context) async -> Timeline<AirQualityEntry> {
        let nextUpdate = Calendar.current.date(
            byAdding: DateComponents(minute: 5),
            to: Date()
        )!
        
        guard let configuredDevice = configuration.device,
              let device = await deviceManager.get(configuredDevice.id) else {
            return Timeline(entries: [], policy: .after(nextUpdate))
        }
        
        do {            
            let sensorData: SensorsDTO = try await withCheckedThrowingContinuation { continuation in
                var cancellable: AnyCancellable?
                
                cancellable = device.getCloudService().fetchMeasurement()
                    .receive(on: DispatchQueue.main)
                    .sink { completion in
                        if case let .failure(error) = completion {
                            continuation.resume(throwing: error)
                        }
                        cancellable?.cancel()
                    } receiveValue: { sensorData in
                        continuation.resume(returning: sensorData)
                        cancellable?.cancel()
                    }
            }
            
            return Timeline(entries: [
                .init(
                    score: 100,
                    temperature: sensorData.temperature,
                    humidity: sensorData.humidity,
                    pressure: sensorData.pressure,
                    eCO2: sensorData.eCO2,
                    date: sensorData.collectedAt
                )
            ], policy: .after(nextUpdate))
        } catch {
            Logger.widgetLogging.error("[AirQualityProvider] Failed to fetch measurement: \(error)")
            
            return Timeline(entries: [], policy: .after(nextUpdate))
        }
    }
}
