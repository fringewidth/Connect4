//
//  BotPlayerServer.cpp
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 10/01/25.
//

#include "BotPlayerServer.h"
#include <thread>
#include <chrono>

BotPlayerServer::BotPlayerServer()
: wsClient("localhost", "8080"), lastMessage({-1, -1}) {}


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

