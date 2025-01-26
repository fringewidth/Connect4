//
//  WebSocketClient.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 10/01/25.
//

#ifndef __WEB_SOCKET_CLIENT_H__

#define __WEB_SOCKET_CLIENT_H__

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <string>
#include <thread>
#include <utility>
#include <mutex>
#include <memory>

auto const SERVER_HOST = "localhost";
auto const SERVER_PORT = "8080";

struct Message {
    int lastMove;
    long long timestamp;
};

enum class GAME_TYPE {
    SERVER_BOT,
    SERVER_PERSON
};

class WebSocketClient { // will be a singleton
    boost::asio::io_context io_context;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
    std::string host;
    std::string port;
    
    bool initializeConnection(GAME_TYPE);


public:
    WebSocketClient(const std::string& host, const std::string& port); // public ctor, need fix
    WebSocketClient(const WebSocketClient&) = delete;
    WebSocketClient& operator=(const WebSocketClient&) = delete;
//    void quitGame(){};
    
    
    static std::unique_ptr<WebSocketClient> instance;
    
    bool isFirst = true;

    
    
    static WebSocketClient& getInstance(const std::string& host=SERVER_HOST, const std::string& port=SERVER_PORT, GAME_TYPE gameType=GAME_TYPE::SERVER_BOT);
//    static WebSocketClient& getInstance();
    
    std::mutex messageMutex;
    ~WebSocketClient();
    
    void listen();
    void connect();
    
    void reset();

    
    Message getLastMessage();
    
    Message sendAndReceiveMove(int);
    Message receiveMove();
    
    Message lastReceivedMessage;
    
};

#endif // __WEB_SOCKET_CLIENT_H__
