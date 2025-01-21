
#include "LoadScreen.h"
#include "constants.h"
#include "MainScene.h"
#include "BotPlayer.h"
#include "BotPlayerServer.h"
#include "BotServerScreen.h"
#include "ConnectingScreen.h"

using namespace ax;


// on "init" you need to initialize your instance
bool LoadScreen::init()
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
    auto playWithYourselfLabel = Label::createWithTTF("Play with Yourself", "fonts/Marker Felt.ttf", FONT_SIZE);
    auto playWithPhoneLabel = Label::createWithTTF("Play with Your Phone", "fonts/Marker Felt.ttf", FONT_SIZE);
    auto playWithServerLabel = Label::createWithTTF("Play with the Server", "fonts/Marker Felt.ttf", FONT_SIZE);
    
    
    // Check if labels are created successfully
    if (playWithYourselfLabel && playWithPhoneLabel && playWithServerLabel)
    {
        // Add shadow effect to each label
        playWithYourselfLabel->enableOutline(Color4B::RED, 2);
        playWithPhoneLabel->enableOutline(Color4B::RED, 2);
        playWithServerLabel->enableOutline(Color4B::RED, 2);


        // Set positions (center vertically and spaced horizontally)
        playWithYourselfLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.75);
        playWithPhoneLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.5);
        playWithServerLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.25);
        
        


        // Add labels to the scene
        this->addChild(playWithYourselfLabel, 3);
        this->addChild(playWithPhoneLabel, 3);
        this->addChild(playWithServerLabel, 3);

        // Create mouse listener
        auto mouseListener = EventListenerMouse::create();
        
        // Event handler for mouse down
        mouseListener->onMouseDown = [playWithYourselfLabel, playWithPhoneLabel, playWithServerLabel, this](EventMouse* event) {
            Vec2 mouseLocation = event->getLocation();

            // Check if each label is clicked
            if (playWithYourselfLabel->getBoundingBox().containsPoint(mouseLocation)) {
                AXLOG("Attempting to create MainScene...");
                auto scene = utils::createInstance<MainScene>();
                if (scene) {
                    AXLOG("MainScene created successfully.");
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
                } else {
                    AXLOG("Failed to create MainScene.");
                }
            }
            else if (playWithPhoneLabel->getBoundingBox().containsPoint(mouseLocation)) {
                AXLOG("Attempting to create BotPlayer...");
                auto scene = utils::createInstance<BotPlayer>();
                if (scene) {
                    AXLOG("BotPlayer created successfully.");
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
                } else {
                    AXLOG("Failed to create BotPlayer.");
                }
            }
            else if (playWithServerLabel->getBoundingBox().containsPoint(mouseLocation)) {
                AXLOG("Attempting to create BotServerScreen...");
//                auto scene = utils::createInstance<BotServerScreen>();
//                if (scene) {
//                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
//                } else {
//                    AXLOG("Failed to create BotServerScreen.");
//                }
                
                auto connectingScreen = ConnectingScreen::create();
                addChild(connectingScreen, 4);
            }
        };

        // Register the event listener
        _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

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

    return true;
}


void LoadScreen::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void LoadScreen::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void LoadScreen::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
//        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void LoadScreen::onMouseDown(Event* event) {

}

void LoadScreen::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void LoadScreen::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
}

void LoadScreen::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
//    AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
}

void LoadScreen::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));
}

void LoadScreen::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
//    AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void LoadScreen::update(float delta)
{
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

void LoadScreen::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/


     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}


