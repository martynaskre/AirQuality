//
//  ContentView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 30/01/2025.
//

import SwiftUI

struct ContentView: View {
    @State var deviceManager = AccessoryManager()
    
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            Text("Hello, world!")
            if deviceManager.activated {
                Button {
                    deviceManager.presentPicker()
                } label: {
                    Text("Add device")
                        .frame(maxWidth: .infinity)
                        .font(Font.headline.weight(.semibold))
                }
                .buttonStyle(.bordered)
                .buttonBorderShape(.roundedRectangle)
                .foregroundStyle(.primary)
                .controlSize(.large)
                .padding(.top, 110)
            }
        }
        .padding()
    }
}

#Preview {
    ContentView()
}
