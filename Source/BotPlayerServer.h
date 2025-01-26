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

class BotPlayerServer : public BotPlayer {
public:
    WebSocketClient& wsClient;
    Message lastMessage;
    
    BotPlayerServer();
    ~BotPlayerServer();
    virtual int askBot() override;
    
    void handleSecondTurn() {
        if(!myTurn){
            placeDisc(wsClient.receiveMove().lastMove);
            myTurn = !myTurn;
        }
    }
    
    virtual void onBackPressed() override;
};

#endif
