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

struct Message {
    int lastMove;
    long long timestamp;
};

class WebSocketClient { // will be a singleton
    boost::asio::io_context io_context;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
    std::string host;
    std::string port;
    
public:
    WebSocketClient(const std::string& host, const std::string& port); // public ctor, need fix
    WebSocketClient(const WebSocketClient&) = delete;
    WebSocketClient& operator=(const WebSocketClient&) = delete;
    
    
    static std::unique_ptr<WebSocketClient> instance;

    
    
    static WebSocketClient& getInstance(const std::string& host, const std::string& port);
    
    std::mutex messageMutex;
    ~WebSocketClient();
    
    void listen();
    void connect();
    
    void reset();

    
    Message getLastMessage();
    
    void sendMove(int);
    
    Message lastReceivedMessage;
    
};

#endif // __WEB_SOCKET_CLIENT_H__
