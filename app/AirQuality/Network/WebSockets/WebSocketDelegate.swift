//
//  WebSocketDelegate.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 16/02/2025.
//

import Foundation

@objc
protocol WebSocketDelegate {
    @objc optional func webSocket(connectionStatus: Bool)
    
    @objc optional func webSocket(didReceiveMessage message: Data)
}
