//
//  BotPlayerServer.cpp
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 10/01/25.
///Users/hrishik/Downloads/Connect4/BotServerScreen.cpp

#include "BotPlayerServer.h"
#include <thread>
#include <chrono>
#include "WebSocketClient.h"

BotPlayerServer::BotPlayerServer()
: lastMessage({-1, -1}), wsClient(WebSocketClient::getInstance("", "")) {
    AXLOG("BotPlayerServer Called.");
}


int BotPlayerServer::askBot() {
    while (true) {
        int userMove = gameBoard.getLastMove().row;
        AXLOG("User move sent: %d", userMove);
        wsClient.sendMove(userMove);
        Message currentMessage = wsClient.getLastMessage();
        AXLOG("received message: %d", currentMessage.lastMove);
        while (currentMessage.timestamp == lastMessage.timestamp) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        lastMessage = currentMessage;
        return lastMessage.lastMove;
    }
}

BotPlayerServer::~BotPlayerServer() {
    wsClient.reset();
}

