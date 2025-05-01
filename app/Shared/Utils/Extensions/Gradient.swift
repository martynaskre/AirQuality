//
//  Gradient.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 17/03/2025.
//

import SwiftUI

public extension Gradient {
    func color(for value: Int) -> Color {
        return self.color(for: Double(value))
    }
    
    func color(for value: Double) -> Color {
        let normalizedValue = min(max(value, 0), 100) / 100
        let numberOfColors = self.stops.count
        
        let index = Int(normalizedValue * Double(numberOfColors - 1))
        
        let fraction = normalizedValue * Double(numberOfColors - 1) - Double(index)
        
        if fraction == 0 {
            return self.stops[index].color
        } else {
            let startColor = self.stops[index].color
            let endColor = self.stops[min(index + 1, numberOfColors - 1)].color
            
            return startColor.interpolate(to: endColor, fraction: fraction)
        }
    }
}
