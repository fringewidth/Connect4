

#include "ServerScreen.h"
#include "constants.h"
#include "MainScene.h"
#include "BotPlayer.h"
#include "BotPlayerServer.h"
#include "ConnectingScreen.h"

using namespace ax;

ServerScreen::ServerScreen() {
    serverLoadingMessage = "Connecting to server";
    errorMessage = "Error connecting :(";
}

void ServerScreen::updateText(Label* messageLabel) {
//    if(connectionFailed) {
//        messageLabel->setString(errorMessage);
//        unschedule("server_connect_key");
//        
//        return;
//    }
    Director::getInstance()->replaceScene(utils::createInstance<BotPlayer>());
    messageDotCount = (messageDotCount + 1) % 4;
    std::string messageText = std::string(serverLoadingMessage);
    
    for(int i = 0; i < messageDotCount; i++){
        messageText += ".";
    }
    
    messageLabel->setString(messageText);
}


// on "init" you need to initialize your instance
bool ServerScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    // Create the three labels
    centerLabel = Label::createWithTTF(serverLoadingMessage, "fonts/Marker Felt.ttf", FONT_SIZE);
    
    
    // Check if labels are created successfully
    if (centerLabel)
    {
        // Add shadow effect to each label
        centerLabel->enableOutline(Color4B::RED, 2);

        // Set positions (center vertically and spaced horizontally)
        center(centerLabel, origin, visibleSize);
        // Add labels to the scene
        this->addChild(centerLabel);

    } else {
        AXLOG("Failed to create labels.");
        return false; // Early return if labels couldn't be created.
    }

    
    
    this->addChild(getBackground(), 0);
//        auto drawNode = DrawNode::create();
//        drawNode->setPosition(Vec2(0, 0));
//        addChild(drawNode);
//
//        drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);
    
    
    
    
    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();
    schedule([this](float dt){
        updateText(this->centerLabel);
    }, 0.5f, "server_connect_key");
    

    return true;
}


void ServerScreen::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void ServerScreen::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void ServerScreen::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void ServerScreen::onMouseDown(Event* event) {

}

void ServerScreen::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void ServerScreen::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
}

void ServerScreen::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
}

void ServerScreen::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));
}

void ServerScreen::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void ServerScreen::update(float delta)
{
    Scene::update(delta);
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {    /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {    /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {    /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    } //switch
}

void ServerScreen::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/


     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}
