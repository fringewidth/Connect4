//
//  BotServerScreen.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 17/01/25.
//
#ifndef __BOT_SERVER_SCREEN_H__
#define __BOT_SERVER_SCREEN_H__
#include "WebSocketClient.h"
#include "ConnectingScreen.h"
#include "WebSocketClient.h"

class BotServerScreen : public ConnectingScreen {
public:
    virtual void connectAndReplaceSelf() {
        WebSocketClient::getInstance("localhost", "8080", GAME_TYPE::SERVER_BOT);

        Director::getInstance()->replaceScene(TransitionFade::create(0.5f,utils::createInstance<BotPlayerServer>()));
    }
};

#endif
