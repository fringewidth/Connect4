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
        
        // Create the label
            // Add the label to the scene
        auto centerLabel = centeredText("Connecting to Server...");
        if(centerLabel) {
            this->addChild(centerLabel);
        } else {
            AXLOG("Failed to create label.");
            return false;
        }

        // Attempt to create the WebSocket client
        try {
            WebSocketClient::getInstance("localhost", "8080"); // Replace with actual server info
            Director::getInstance()->replaceScene(utils::createInstance<BotPlayerServer>());
        } catch (const std::runtime_error& e) {
            // Handle the error and update the label with the failure message
            updateLabel(centerLabel, "Server Unavailable :(");
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
            ret->autorelease();  // Add to autorelease pool
            return ret;
        } else {
            delete ret; // Clean up if creation failed
            return nullptr;
        }
    }

private:
    // Helper method to update the label text
    void updateLabel(Label* label, const std::string& newMessage) {
        if (label) {
            label->setString(newMessage);
        }
    }
};
