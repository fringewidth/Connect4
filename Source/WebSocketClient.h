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

struct Message {
    int lastMove;
    long long timestamp;
};

class WebSocketClient {
    boost::asio::io_context io_context;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
    std::string host;
    std::string port;
    
public:
    WebSocketClient(const std::string& host, const std::string& port);
    
    std::mutex messageMutex;
    ~WebSocketClient();
    
    void listen();
    void connect();
    
    Message getLastMessage();
    
    void sendMove(int);
    
    Message lastReceivedMessage;
    
};

#endif // __WEB_SOCKET_CLIENT_H__
