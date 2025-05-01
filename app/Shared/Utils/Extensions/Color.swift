//
//  Color.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 17/03/2025.
//

import SwiftUI

public extension Color {
    func interpolate(to endColor: Color, fraction: Double) -> Color {
        let startComponents = UIColor(self).cgColor.components ?? [0, 0, 0, 0]
        let endComponents = UIColor(endColor).cgColor.components ?? [0, 0, 0, 0]
        
        let r = startComponents[0] + (endComponents[0] - startComponents[0]) * CGFloat(fraction)
        let g = startComponents[1] + (endComponents[1] - startComponents[1]) * CGFloat(fraction)
        let b = startComponents[2] + (endComponents[2] - startComponents[2]) * CGFloat(fraction)
        let a = startComponents[3] + (endComponents[3] - startComponents[3]) * CGFloat(fraction)
        
        return Color(red: Double(r), green: Double(g), blue: Double(b), opacity: Double(a))
    }
}
