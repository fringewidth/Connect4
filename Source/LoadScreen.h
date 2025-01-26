//
//  LoadScreen.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 16/01/25.
//

#ifndef __LOAD_SCREEN_H__
#define __LOAD_SCREEN_H__

#include<axmol.h>
#include "LoadScreen.h"
#include "constants.h"
#include "MainScene.h"
#include "BotPlayer.h"
#include "BotPlayerServer.h"
#include "BotServerScreen.h"
#include "ConnectingScreen.h"

class LoadScreen : public ax::Scene {
private:
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };
    
private:
    GameState _gameState = GameState::init;
    
public:
    bool init() override;
    void update(float delta) override;
    
    // touch
    void onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);
    
    // mouse
    virtual void onMouseDown(ax::Event* event);
    void onMouseUp(ax::Event* event);
    void onMouseMove(ax::Event* event);
    void onMouseScroll(ax::Event* event);
    
    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);
    
    // a selector callback
    void menuCloseCallback(ax::Object* sender);
    
    void playCallBack();
    
    
    void loadConnectingScreen(GAME_TYPE);

};

#endif
