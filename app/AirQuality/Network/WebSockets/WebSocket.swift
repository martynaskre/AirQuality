//
//  WebSocket.swift
//  AirQuality
//
//  Created by Martynas Skrebė on 16/02/2025.
//

import Foundation

class WebSocket: NSObject {
    private let url: URL
    private var session: URLSession?
    private var socket: URLSessionWebSocketTask?
    
    weak var delegate: WebSocketDelegate?
    
    private(set) var connected: Bool = false {
        didSet {
            self.delegate?.webSocket?(connectionStatus: self.connected)
        }
    }
    
    init(_ url: URL) {
        self.url = url
        
        super.init()
    }
    
    func connect() {
        self.session = URLSession(configuration: .default, delegate: self, delegateQueue: OperationQueue())
        self.socket = session?.webSocketTask(with: URLRequest(url: url))
        self.socket?.resume()
        self.receive()
    }
    
    private func receive() {
        self.socket?.receive { result in
            switch result {
            case .failure:
                self.connected = false
            case .success(let message):
                switch message {
                case .string(let text):
                    if let textAsData = text.data(using: .utf8) {
                        self.delegate?.webSocket?(didReceiveMessage: textAsData)
                    }
                                        
                    break
                case .data(let data):
                    self.delegate?.webSocket?(didReceiveMessage: data)
                    
                    break
                @unknown default:
                    break
                }
                
                self.receive()
            }
        }
    }
    
    func disconnect() {
        socket?.cancel(with: .goingAway, reason: nil)
    }
}

/// MARK: URLSessionWebSocketDelegate
extension WebSocket: URLSessionWebSocketDelegate {
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        self.connected = true
    }

    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        self.connected = false
    }
}
