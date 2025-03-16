//
//  AirQualityWidgetView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/03/2025.
//

import SwiftUI

struct AirQualityWidgetView : View {
    var entry: AirQualityProvider.Entry

    var body: some View {
        HStack(alignment: .center) {
            VStack(spacing: 20) {
                Gauge(value: Float(entry.score), in: 0...100) {} currentValueLabel: {
                    Text("\(entry.score)")
                        .foregroundColor(Color.green)
                }
                .gaugeStyle(.accessoryCircular)
                .tint(Gradient(colors: [.red, .orange, .yellow, .green]))
                .scaleEffect(1.5)
                Text("Air Quality Score")
                    .font(.subheadline)
                    .bold()
            }
            .padding(.top, 13)
            Spacer()
            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 16) {
                VStack {
                    Text("Temperature")
                        .font(.system(size: 11))
                    Text(entry.temperature.formatted())
                        .font(.system(size: 12))
                        .bold()
                }
                .frame(maxWidth: .infinity, minHeight: 60)
                .background(Color.gray.opacity(0.2))
                .cornerRadius(12)
                
                VStack {
                    Text("Humidity")
                        .font(.system(size: 11))
                    Text("\(entry.humidity, specifier: "%.1f") %")
                        .font(.system(size: 12))
                        .bold()
                }
                .frame(maxWidth: .infinity, minHeight: 60)
                .background(Color.gray.opacity(0.2))
                .cornerRadius(12)
                
                VStack {
                    Text("Pressure")
                        .font(.system(size: 11))
                    Text(entry.pressure.formatted())
                        .font(.system(size: 12))
                        .bold()
                }
                .frame(maxWidth: .infinity, minHeight: 60)
                .background(Color.gray.opacity(0.2))
                .cornerRadius(12)
                
                VStack {
                    Text("eCO2")
                        .font(.system(size: 11))
                    Text("\(entry.eCO2, specifier: "%.0f") ppm")
                        .font(.system(size: 12))
                        .bold()
                }
                .frame(maxWidth: .infinity, minHeight: 60)
                .background(Color.gray.opacity(0.2))
                .cornerRadius(12)
            }
        }
    }
}
