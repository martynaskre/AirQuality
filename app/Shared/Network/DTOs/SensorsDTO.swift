//
//  SensorsDTO.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import Foundation

public struct SensorsDTO: Decodable {
    public enum CodingKeys: CodingKey {
        case lux, temperature, pressure, humidity, eco2, tvoc, collectedAt
    }
    
    public var score: Int {
        let temperatureScore = max(0, 100 - max(0, abs(temperature.value - 22) - 2) * 20)
        let humidityScore = max(0, 100 - abs(humidity - 40) * 2)
        let eco2Score = max(0, 100 - max(eCO2 - 400, 0) / 10)
        let tvocScore = max(0, 100 - max(TVOC - 400, 0) / 5)
        
        return Int(temperatureScore * 0.25 + humidityScore * 0.25 + eco2Score * 0.25 + tvocScore * 0.25)
    }
    
    public let lux: Measurement<UnitIlluminance>
    public let temperature: Measurement<UnitTemperature>
    public let pressure: Measurement<UnitPressure>
    public let humidity: Double
    public let eCO2: Double
    public let TVOC: Double
    public let collectedAt: Date
    
    public init(from decoder: any Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        
        self.lux = Measurement(value: try container.decode(Double.self, forKey: .lux), unit: .lux)
        self.temperature = Measurement(value: try container.decode(Double.self, forKey: .temperature), unit: .celsius)
        self.pressure = Measurement(value: try container.decode(Double.self, forKey: .pressure), unit: .newtonsPerMetersSquared)
        self.humidity = try container.decode(Double.self, forKey: .humidity)
        self.eCO2 = try container.decode(Double.self, forKey: .eco2)
        self.TVOC = try container.decode(Double.self, forKey: .tvoc)
        
        do {
            self.collectedAt = try container.decode(Date.self, forKey: .collectedAt)
        } catch {
            self.collectedAt = Date()
        }
    }
}
