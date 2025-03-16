//
//  DataController.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import Foundation
import SwiftData
import Network

@MainActor
public class DataController {
#if DEBUG
    public static let shared = DataController(inMemory: true, initPreviewData: ProcessInfo.processInfo.isPreview)
#else
    public static let shared = DataController()
#endif
    
    private static let schema = Schema([
        Device.self
    ])
    
    public let container: ModelContainer
    
    public init(inMemory: Bool = false, initPreviewData: Bool = false) {
        let modelConfiguration = ModelConfiguration(schema: DataController.schema, isStoredInMemoryOnly: inMemory)

        do {
            container = try ModelContainer(for: DataController.schema, configurations: [modelConfiguration])
            
            if initPreviewData {
                initializePreviewData()
            }
        } catch {
            fatalError("Could not create ModelContainer: \(error)")
        }
    }
    
    private func initializePreviewData() {
        for i in 1...5 {
            let device = Device(name: "Device \(i)", accessoryId: UUID(), serialNumber: UUID())
            
            container.mainContext.insert(device)
        }
    }
}
