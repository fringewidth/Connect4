//  WebSocketClient.cpp
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 10/01/25.

#include "WebSocketClient.h"
#include <chrono>
#include <boost/json/src.hpp>
#include <iostream>
#include "axmol.h"
#include <memory>


std::unique_ptr<WebSocketClient> WebSocketClient::instance = nullptr;


WebSocketClient::WebSocketClient(const std::string& host, const std::string& port)
    : host(host), port(port), ws(io_context) {
    connect();
    listen();
}

WebSocketClient::~WebSocketClient() {
    try {
        ws.close(boost::beast::websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Error closing WebSocket: " << e.what() << std::endl;
    }
}

WebSocketClient& WebSocketClient::getInstance(const std::string& host="", const std::string& port="") {
    if(instance == nullptr) {
        if (host == "" || port == "") {
            throw std::runtime_error("Host and port must be specified at the first init");
        }
        instance = std::make_unique<WebSocketClient>(host, port);
    }
    
    return *instance;
}

void WebSocketClient::connect() {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(host, port);
        boost::asio::connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake(host, "/");
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to connect to the WebSocket server: " + std::string(e.what()));
    }
}

void WebSocketClient::listen() {
//    std::thread([this]() {
//        while (true) {
//            try {
//                boost::beast::flat_buffer buffer;
//                ws.read(buffer);
//                auto data = buffer.data();
//                std::string receivedString(static_cast<const char*>(data.data()), data.size());
//                
//                auto receivedJSON = boost::json::parse(receivedString);
//                Message message{
//                    static_cast<int>(receivedJSON.at("lastMove").as_int64()),
//                    receivedJSON.at("timestamp").as_int64()
//                };
//                std::lock_guard<std::mutex> lock(messageMutex);
//                lastReceivedMessage = message;
//            } catch (const std::exception& e) {
//                std::cerr << "Error reading message: " << e.what() << std::endl;
//            }
//        }
//    }).detach();
}

Message WebSocketClient::getLastMessage() {
//    std::lock_guard<std::mutex> lock(messageMutex);
    return lastReceivedMessage;
}

void WebSocketClient::sendMove(int move) {
    try {
        auto now = std::chrono::system_clock::now();
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();

        boost::json::object jsonMessage;
        jsonMessage["lastMove"] = move;
        jsonMessage["timestamp"] = timestamp;

        std::string stringMessage = boost::json::serialize(jsonMessage);
        ws.write(boost::asio::buffer(stringMessage));
    } catch (const std::exception& e) {
        std::cerr << "Error sending move: " << e.what() << std::endl;
    }
        
    auto newReceivedMessage = lastReceivedMessage;
    while (lastReceivedMessage.timestamp == newReceivedMessage.timestamp) {
        try {
            boost::beast::flat_buffer buffer;
            ws.read(buffer);
            auto data = buffer.data();
            std::string receivedString(static_cast<const char*>(data.data()), data.size());

            auto receivedJSON = boost::json::parse(receivedString);
            Message message{
                static_cast<int>(receivedJSON.at("lastMove").as_int64()),
                receivedJSON.at("timestamp").as_int64()
            };
//                std::lock_guard<std::mutex> lock(messageMutex);
            newReceivedMessage = message;
        } catch (const std::exception& e) {
            std::cerr << "Error reading message: " << e.what() << std::endl;
        }
    }
    lastReceivedMessage = newReceivedMessage;


}
