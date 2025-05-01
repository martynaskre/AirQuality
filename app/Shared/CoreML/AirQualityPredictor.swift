//
//  AirQualityScore.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 17/03/2025.
//

import CoreML

public class AirQualityPredictor {
    public static let shared = AirQualityPredictor()
    
    private let model: AirQualityScore
    
    init() {
        do {
            model = try AirQualityScore(configuration: .init())
        } catch {
            fatalError()
        }
    }
    
    public func predict(lux: Double, temperature: Double, pressure: Double, humidity: Double, eCO2: Double, TVOC: Double) -> Int {
        do {
            let prediction = try model.prediction(input: .init(lux: lux, temperature: temperature, pressure: pressure, humidity: humidity, eco2: eCO2, tvoc: TVOC))
            
            return Int(round(prediction.score))
        } catch {
            print("Error making prediction: \(error)")
        }
        
        return 0
    }
}
