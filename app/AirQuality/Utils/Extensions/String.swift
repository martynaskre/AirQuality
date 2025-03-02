//
//  String.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 08/02/2025.
//

import Foundation

extension String {
    func padNull(toLength length: Int) -> Data {
        var buffer = [UInt8](repeating: 0, count: length)
        
        let utf8Bytes = Array(self.utf8.prefix(length))
        
        buffer.replaceSubrange(0..<utf8Bytes.count, with: utf8Bytes)
        
        return Data(buffer)
    }
}
