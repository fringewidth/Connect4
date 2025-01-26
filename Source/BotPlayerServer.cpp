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
#include "constants.h"

BotPlayerServer::BotPlayerServer()
: lastMessage({-1, -1}), wsClient(WebSocketClient::getInstance()) {}

int BotPlayerServer::askBot() {
    int userMove = gameBoard.getLastMove().row;
    if(gameBoard.isGameOver()) {
        wsClient.sendAndReceiveMove(userMove); // let it know game over
        return -1;
    }
    AXLOG("User move sent: %d", userMove);
//    Message currentMessage = wsClient.receiveMove();
    auto currentMessage = wsClient.sendAndReceiveMove(userMove);
    return currentMessage.lastMove;
}

BotPlayerServer::~BotPlayerServer() {
    wsClient.reset();
}


void BotPlayerServer::onBackPressed() {
//    wsClient.quitGame();
}
