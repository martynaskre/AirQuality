//
//  DeviceView.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 15/02/2025.
//

import SwiftUI
import Charts
import Shared

struct DeviceView: View {
    @Environment(\.safeAreaInsets) private var insets: EdgeInsets
    
    @State var viewModel: ViewModel
    
    private let gaugeGradient = Gradient(colors: [.red, .orange, .yellow, .green])
    
    init(_ device: Device) {
        self._viewModel = .init(wrappedValue: .init(device))
    }
    
    var body: some View {
        VStack(spacing: 0) {
            VStack(alignment: .center, spacing: 10) {}
            .padding(.top, insets.top)
            .padding(.horizontal)
            .padding(.bottom, 70)
            .frame(maxWidth: .infinity)
            .background(
                LinearGradient(gradient: Gradient(colors: [Color.accentColor, Color.accentColor.opacity(0)]), startPoint: .top, endPoint: .bottom)
            )
            .ignoresSafeArea(.container, edges: .top)
            ScrollView {
                VStack(spacing: 20) {
                    Gauge(value: Double(viewModel.sensorData?.score ?? 0), in: 0...100) {} currentValueLabel: {
                        Text("\(viewModel.sensorData?.score ?? 0)")
                            .foregroundStyle(gaugeGradient.color(for: viewModel.sensorData?.score ?? 0))
                    }
                    .gaugeStyle(.accessoryCircular)
                    .tint(gaugeGradient)
                    .scaleEffect(2)
                    Text("Air Quality Score")
                        .font(.headline)
                }
                .padding(.top, 30)
                .padding(.bottom)
                
                HStack(alignment: .center) {
                    Text("Raw Data")
                        .font(.title2)
                        .bold()
                    Spacer()
                    if viewModel.liveModeProgress {
                        ProgressView()
                            .controlSize(.mini)
                    }
                    Button(viewModel.liveModeActive ? "Regular mode" : "Live mode", action: {
                        viewModel.toggleLiveMode()
                    })
                    .disabled(viewModel.liveModeProgress || viewModel.liveModeAvailable != true)
                }
                .padding(.horizontal)
                
                rawDataBody
            }
            .refreshable {
                viewModel.fetchSensors()
                
                try? await Task.sleep(for: .milliseconds(500))
            }
            .padding(.top, -50)
            Spacer()
        }
        .navigationTitle(viewModel.device.name)
    }
    
    private var rawDataBody: some View {
        Group {
            if let sensorData = viewModel.sensorData {
                LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 16) {
                    VStack {
                        Text("Illuminance")
                            .font(.headline)
                        Text(sensorData.lux.formatted())
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                    
                    VStack {
                        Text("Temperature")
                            .font(.headline)
                        Text(sensorData.temperature.formatted())
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                    
                    VStack {
                        Text("Pressure")
                            .font(.headline)
                        Text(sensorData.pressure.humanFormat())
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                    
                    VStack {
                        Text("Humidity")
                            .font(.headline)
                        Text("\(sensorData.humidity, specifier: "%.1f") %")
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                    
                    VStack {
                        Text("eCO2")
                            .font(.headline)
                        Text("\(sensorData.eCO2, specifier: "%.1f") ppm")
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                    
                    VStack {
                        Text("TVOC")
                            .font(.headline)
                        Text("\(sensorData.TVOC, specifier: "%.1f") ppb")
                            .font(.title2)
                            .bold()
                    }
                    .frame(maxWidth: .infinity, minHeight: 80)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(12)
                }
                .padding(.top)
                .padding(.horizontal)
                .padding(.bottom, 5)
                HStack {
                    Spacer()
                    Text("Collected \(Date().diffForHumans(to: sensorData.collectedAt))")
                        .font(.footnote)
                        .foregroundColor(.secondary)
                }
                .padding(.horizontal, 20)
            } else {
                HStack {
                    Text("No data available")
                        .font(.footnote)
                        .foregroundColor(.secondary)
                    Spacer()
                }
                .padding()
            }
        }
    }
}

#Preview {
    let device = Device(
        name: "Station",
        accessoryId: UUID(),
        serialNumber: UUID()
    )
    
    return DeviceView(device)
}
