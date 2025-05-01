//
//  Measurements.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 22/03/2025.
//

import Foundation

public extension Measurement where UnitType: UnitPressure {
    func humanFormat() -> String {
        let formatter = MeasurementFormatter()
        formatter.unitStyle = .medium
        formatter.unitOptions = .providedUnit
        
        let numberFormatter = NumberFormatter()
        numberFormatter.minimumFractionDigits = 0
        numberFormatter.maximumFractionDigits = 0

        formatter.numberFormatter = numberFormatter

        return formatter.string(from: self.converted(to: UnitPressure.hectopascals as! UnitType))
    }
}
