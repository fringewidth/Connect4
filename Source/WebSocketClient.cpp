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

void WebSocketClient::reset() {
    try {
        if (ws.is_open()) {
            boost::system::error_code ec;
            ws.close(boost::beast::websocket::close_code::going_away, ec);
            if (ec) {
                std::cerr << "Error closing WebSocket: " << ec.message() << std::endl;
            }
        }
        io_context.stop();
    } catch (const std::exception& e) {
        std::cerr << "Exception during WebSocket close: " << e.what() << std::endl;
    }

    instance.reset(); 
}

WebSocketClient::~WebSocketClient() {
    try {
        ws.close(boost::beast::websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Error closing WebSocket: " << e.what() << std::endl;
    }
}

WebSocketClient& WebSocketClient::getInstance(const std::string& host, const std::string& port, GAME_TYPE gameType) {
    if(instance == nullptr) {
        instance = std::make_unique<WebSocketClient>(host, port);
        instance->initializeConnection(gameType);
    }
    
    return *instance;
}

WebSocketClient& WebSocketClient::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("Host and port must be provided on the first init.");
    }
    return *instance;
}

bool WebSocketClient::initializeConnection(GAME_TYPE gameType) { // returns if client is first
    try {
        boost::json::object initMessage;
        initMessage["type"] = "init";

        if (gameType == GAME_TYPE::SERVER_BOT) {
            initMessage["mode"] = "BOT";
            std::string messageString = boost::json::serialize(initMessage);
            ws.write(boost::asio::buffer(messageString));

            return true;
        } else if (gameType == GAME_TYPE::SERVER_PERSON) {
            initMessage["mode"] = "PERSON";

            std::string messageString = boost::json::serialize(initMessage);
            ws.write(boost::asio::buffer(messageString));

            boost::beast::flat_buffer buffer;
            ws.read(buffer);

            auto data = buffer.data();
            std::string responseString(static_cast<const char*>(data.data()), data.size());
            auto responseJSON = boost::json::parse(responseString).as_object();

            if (responseJSON["type"].as_string() == "timeout") {
                throw std::runtime_error("No one on the other side :(");
            } else if (responseJSON["type"].as_string() == "accept") {
                int order = std::stoi(std::string(responseJSON["order"].as_string()));
                AXLOG("Game accepted. Order: %d", order);
                return (order == 0);
            } else {
                throw std::runtime_error("Unexpected server response");
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Initialization failed: \n") + e.what());
    }
    return false;
}

void WebSocketClient::connect() {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(host, port);
        boost::asio::connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake(host, "/");
    } catch (const std::exception& e) {
        throw std::runtime_error("Server Unavailable :(");
    }
}

void WebSocketClient::listen() {}

Message WebSocketClient::getLastMessage() {
//    std::lock_guard<std::mutex> lock(messageMutex);
    return lastReceivedMessage;
}

//Message WebSocketClient::sendMove(int move) {
//    try {
//        auto now = std::chrono::system_clock::now();
//        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
//            now.time_since_epoch()).count();
//
//        boost::json::object jsonMessage;
//        jsonMessage["lastMove"] = move;
//        jsonMessage["timestamp"] = timestamp;
//
//        std::string stringMessage = boost::json::serialize(jsonMessage);
//        ws.write(boost::asio::buffer(stringMessage));
//    } catch (const std::exception& e) {
//        std::cerr << "Error sending move: " << e.what() << std::endl;
//    }
//
//    try {
//        boost::beast::flat_buffer buffer;
//        ws.read(buffer);
//        auto data = buffer.data();
//        std::string receivedString(static_cast<const char*>(data.data()), data.size());
//
//        auto receivedJSON = boost::json::parse(receivedString);
//        Message message{
//            static_cast<int>(receivedJSON.at("lastMove").as_int64()),
//            receivedJSON.at("timestamp").as_int64()
//        };
//
//        lastReceivedMessage = message;
//        return message;
//    } catch (const std::exception& e) {
//        std::cerr << "Error reading message: " << e.what() << std::endl;
//        return {};
//    }
//}


Message WebSocketClient::sendMove(int move) {
    try {
        // Send the move as usual
        auto now = std::chrono::system_clock::now();
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();

        boost::json::object jsonMessage;
        jsonMessage["lastMove"] = move;
        jsonMessage["timestamp"] = timestamp;

        std::string stringMessage = boost::json::serialize(jsonMessage);

        // Log the message being sent
        std::cout << "Sending move: " << stringMessage << std::endl;

        ws.write(boost::asio::buffer(stringMessage));
    } catch (const std::exception& e) {
        std::cerr << "Error sending move: " << e.what() << std::endl;
    }

    try {
        boost::beast::flat_buffer buffer;
        ws.read(buffer);
        auto data = buffer.data();
        std::string receivedString(static_cast<const char*>(data.data()), data.size());

        // Log the received raw message
        std::cout << "Received raw message: " << receivedString << std::endl;

        auto receivedJSON = boost::json::parse(receivedString);
        
        // Check if the response contains "type" and if it is "gameOver"
        if (receivedJSON.is_object()) {
            auto jsonObject = receivedJSON.as_object();
            auto typeIt = jsonObject.find("type");
            if (typeIt != jsonObject.end() && typeIt->value() == "gameOver") {
                auto winnerIt = jsonObject.find("winner");
                if (winnerIt != jsonObject.end()) {
                    std::string winner = winnerIt->value().as_string().c_str();
                    std::cout << "Game Over. Winner: " << winner << std::endl;
                }
                return {};  // Returning an empty message to indicate game over
            }
        }

        // Parse the move from the server's response
        Message message{
            static_cast<int>(receivedJSON.at("lastMove").as_int64()),
            receivedJSON.at("timestamp").as_int64()
        };

        // Log the parsed message
        std::cout << "Parsed message - lastMove: " << message.lastMove
                  << ", timestamp: " << message.timestamp << std::endl;

        lastReceivedMessage = message;
        return message;

    } catch (const std::exception& e) {
        std::cerr << "Error reading message: " << e.what() << std::endl;
        return {};
    }
}
