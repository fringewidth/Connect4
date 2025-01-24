//
//  ConnectingScreen.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 21/01/25.
//

#include "axmol.h"
#include "constants.h"
#include "WebSocketClient.h"
#include "BotPlayerServer.h"
#pragma once
using namespace ax;

class ConnectingScreen : public ax::Layer {
public:
    virtual bool init() override {
        // Create background
        auto background = getBackground();
        if (background) {
            this->addChild(background);
        } else {
            AXLOG("Failed to load background.");
        }
        
        auto centerLabel = centeredText("Connecting to Server...");
        if(centerLabel) {
            this->addChild(centerLabel);
        } else {
            AXLOG("Failed to create label.");
            return false;
        }

        try {
            WebSocketClient::getInstance("localhost", "8080", GAME_TYPE::SERVER_BOT);
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f,utils::createInstance<BotPlayerServer>()));
        } catch (const std::runtime_error& e) {
            updateLabel(centerLabel, e.what());
            auto wait = ax::DelayTime::create(1.0f);
            auto removeSelf = ax::RemoveSelf::create();
            auto seq = Sequence::create(wait, removeSelf, NULL);
            this->runAction(seq);
        }

        return true; // Make sure to return true if initialization succeeds
    }

    static ConnectingScreen* create() {
        ConnectingScreen* ret = new (std::nothrow) ConnectingScreen();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        } else {
            delete ret;
            return nullptr;
        }
    }

private:
    void updateLabel(Label* label, const std::string& newMessage) {
        if (label) {
            label->setString(newMessage);
        }
    }
};
