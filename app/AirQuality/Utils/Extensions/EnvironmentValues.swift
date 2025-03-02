//
//  EnvironmentValues.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/02/2025.
//

import SwiftUI

extension EnvironmentValues {
    private struct SafeAreaInsetsKey: EnvironmentKey {
        static var defaultValue: EdgeInsets {
            let keyWindow = UIApplication.shared.keyWindow
            
            return keyWindow?.safeAreaInsets.edgeInsets ?? EdgeInsets()
        }
    }
    
    var safeAreaInsets: EdgeInsets {
        self[SafeAreaInsetsKey.self]
    }
}
