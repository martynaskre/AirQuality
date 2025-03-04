//
//  Date.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 04/03/2025.
//

import Foundation

extension Date {
    func diffForHumans(to date: Date) -> String {
        let formatter = RelativeDateTimeFormatter()
        formatter.unitsStyle = .full
        formatter.dateTimeStyle = .numeric
        
        return formatter.localizedString(for: date, relativeTo: self)
    }
}
