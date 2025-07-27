//
//  BotPlayerServer.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 10/01/25.
//
#ifndef __BOT_PLAYER_SERVER_H__
#define __BOT_PLAYER_SERVER_H__
#include "BotPlayer.h"
#include "WebSocketClient.h"
#include "ServerScreen.h"
#include "constants.h"

class BotPlayerServer : public BotPlayer {
public:
    WebSocketClient& wsClient;
    Message lastMessage;
    
    BotPlayerServer();
    ~BotPlayerServer();
    virtual int askBot() override;
    
    int handleSecondTurn() {
        if(!myTurn){
            auto lastMove = wsClient.receiveMove().lastMove;
            if(lastMove != FORFEIT_CODE){
                placeDisc(lastMove);
                myTurn = !myTurn;
            }
            else {
                throw std::runtime_error("Your opoponent got cold feet.\nTry again.");
            }
        }
        return 0;
    }
    
    virtual void onBackPressed() override;
};

#endif
