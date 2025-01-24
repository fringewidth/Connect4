
#include "LoadScreen.h"
#include "constants.h"
#include "MainScene.h"
#include "BotPlayer.h"
#include "BotPlayerServer.h"
#include "BotServerScreen.h"
#include "ConnectingScreen.h"

using namespace ax;

static void loadConnectingScreen(LoadScreen &object, GAME_TYPE gt) {
    auto connectingScreen = ConnectingScreen::create();
    object.addChild(connectingScreen, 4);
    connectingScreen->setGameAndLoad(gt);
}

bool LoadScreen::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    std::vector<float> positions = {0.8, 0.6, 0.4, 0.2};
    std::vector<std::string> texts = {
        "Play with Yourself",
        "Play with Your Phone",
        "Play with the Server",
        "Play Online"
    };
    std::vector<Label*> labels(4);
    
    for (size_t i = 0; i < texts.size(); ++i) {
        Vec2 position(visibleSize.width / 2, visibleSize.height * positions[i]);
        auto label = createStyledLabel(texts[i], position);
        if (label) {
            this->addChild(label, 3);
            labels[i] = label;
        } else {
            AXLOG("Failed to create labels.");
            return false;
        }
    }

    auto mouseListener = EventListenerMouse::create();
        
    mouseListener->onMouseDown = [labels, this](EventMouse* event) {
        Vec2 mouseLocation = event->getLocation();

        if (isPointIn(labels[0], mouseLocation)) { // play with yourself
                AXLOG("Attempting to create MainScene...");
                auto scene = utils::createInstance<MainScene>();
                if (scene) {
                    AXLOG("MainScene created successfully.");
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
                } else {
                    AXLOG("Failed to create MainScene.");
                }
            }
            
            
        else if (isPointIn(labels[1], mouseLocation)) { // play with bot
                AXLOG("Attempting to create BotPlayer...");
                auto scene = utils::createInstance<BotPlayer>();
                if (scene) {
                    AXLOG("BotPlayer created successfully.");
                    Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
                } else {
                        AXLOG("Failed to create BotPlayer.");
                }
            }
            
            
        else if (isPointIn(labels[2], mouseLocation)) { // play with server
            loadConnectingScreen(*this, GAME_TYPE::SERVER_BOT);
        }
        
        else if (isPointIn(labels[3], mouseLocation)) { // play online
            loadConnectingScreen(*this, GAME_TYPE::SERVER_PERSON);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    this->addChild(getBackground(), 0);

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


