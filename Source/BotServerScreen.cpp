//
//  BotServerScreen.cpp
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 17/01/25.
//

#include "BotServerScreen.h"
#include "WebSocketClient.h"
#include "BotPlayerServer.h"
#include "LoadScreen.h"

using namespace ax;

BotServerScreen::BotServerScreen() {
    serverLoadingMessage = "Readying up the server";
    errorMessage = "Server Unavailable :(";

    try{
        AXLOG("replacing scene");
        WebSocketClient::getInstance("localhost", "8080"); // initalise server connection (ignore if server already initalized)
        Director::getInstance()->replaceScene(utils::createInstance<MainScene>());
        AXLOG("scene not replaced");

    } catch (const std::runtime_error& e) {
            connectionFailed = true; // catch server unavailable error
    }
}
