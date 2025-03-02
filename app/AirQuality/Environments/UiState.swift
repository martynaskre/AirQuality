//
//  UiState.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 31/01/2025.
//

import SwiftUI

@Observable
class UiState {
    enum ApplicationRoute: Hashable {
        case pairing(_ pairingSession: PairingSession)
        case device(_ device: Device)
    }
    
    var navigationPath = [ApplicationRoute]()
}
