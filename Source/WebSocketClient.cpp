#include "WebSocketClient.h"
#include <chrono>
#include <iostream>
#include "axmol.h"
#include <memory>
#include <boost/json/src.hpp>




std::unique_ptr<WebSocketClient> WebSocketClient::instance = nullptr;


WebSocketClient::WebSocketClient(const std::string& host, const std::string& port)
    : host(host), port(port), ws(io_context) {
    connect();
}

void WebSocketClient::reset() {
    isFirst = true;
    host = SERVER_HOST;
    port = SERVER_PORT;
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
        instance->isFirst = instance->initializeConnection(gameType);
    }
    
    return *instance;
}

void serializeAndSend(boost::json::object message, boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws) {
    const std::string messageString = boost::json::serialize(message);
    ws.write(boost::asio::buffer(messageString));
}

boost::json::object receiveAndParse(boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws) {
    boost::beast::flat_buffer buffer;
    ws.read(buffer);
    auto data = buffer.data();
    std::string responseString(static_cast<const char*>(data.data()), data.size());
    auto responseJSON = boost::json::parse(responseString).as_object();
    return responseJSON;
}


static void resetAndThrow(const std::string& errMsg) {
    WebSocketClient::getInstance().reset();
    throw std::runtime_error(errMsg);
}

bool WebSocketClient::initializeConnection(GAME_TYPE gameType) { // returns if client is first
    try {
        boost::json::object initMessage = {
            {"type", "init"},
            {"mode",
                gameType==GAME_TYPE::SERVER_PERSON ? "PERSON" : "BOT"
            }
        };
        
        serializeAndSend(initMessage, ws);
        auto responseJSON = receiveAndParse(ws);

        if (gameType == GAME_TYPE::SERVER_PERSON && responseJSON["type"].as_string() == "timeout") {
            resetAndThrow("No one on the other side :(");
            return true; // error means reset to default ig
        } // only happens when mode is person
        
        else if (responseJSON["type"].as_string() == "accept") {
            int order = std::stoi(std::string(responseJSON["order"].as_string()));
            AXLOG("Game accepted. Order: %d", order);
            return (order == 0);
        }
        
        else {
            resetAndThrow("Server busy :(");
        }
        
    } catch (const std::exception& e) {
        resetAndThrow(e.what());
    }
    
    return true; // should be dead code
}

void WebSocketClient::connect() {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto const results = resolver.resolve(host, port);
        boost::asio::connect(ws.next_layer(), results.begin(), results.end());
        ws.handshake(host, "/");
    } catch (const std::exception& e) {
        resetAndThrow("Server Unavailable :(");
    }
}

void WebSocketClient::listen() {}

Message WebSocketClient::getLastMessage() {
    return lastReceivedMessage;
}

Message WebSocketClient::sendAndReceiveMove(int move) {
    try {
        // Send the move as usual
        auto now = std::chrono::system_clock::now();
        long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();

        boost::json::object moveJSON = {
            { "lastMove", move },
            { "timestamp", timestamp }
        };
  
        AXLOG("sending: %d", move);

        serializeAndSend(moveJSON, ws);
    } catch (const std::exception& e) {
        std::cerr << "Error sending move: " << e.what() << std::endl;
    }

    return receiveMove();
}

bool safeCheckValue(const boost::json::object obj, const std::string& key, const std::string& value) {
    auto typeIt = obj.find(key);
    if(typeIt == obj.end() || typeIt->value().as_string() != value) {
        return false;
    }
    return true;
}

Message WebSocketClient::receiveMove() {
    try {
        
        auto receivedJSON = receiveAndParse(ws);

        
        if (safeCheckValue(receivedJSON, "type", "gameOver")) {
            std::string winner = std::string(receivedJSON["winner"].as_string());
            std::cout << "Game Over. Winner: " << winner << std::endl;
            return {};
        }
        
        if (!receivedJSON.contains("lastMove") || !receivedJSON.contains("timestamp")) {
            throw std::runtime_error("Missing required fields: 'lastMove' or 'timestamp'");
        }

        
        Message message{
            static_cast<int>(receivedJSON["lastMove"].as_int64()),
            receivedJSON["timestamp"].as_int64()
        };

        std::cout << "Parsed message - lastMove: " << message.lastMove
                  << ", timestamp: " << message.timestamp << std::endl;

        
        lastReceivedMessage = message;
        return message;

    } catch (const std::exception& e) {
        std::cerr << "Error reading message: " << e.what() << std::endl;
        return {};
    }
}


