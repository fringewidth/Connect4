/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "axmol.h"
#include "GameBoard.h"
#include "constants.h"

class EndScreen : public Layer {
    Label* overLabel;

public:
    virtual bool init() override {
        auto background = getBackground();
        overLabel = centeredText("Game Over!");

        if (background && overLabel) {
            this->addChild(background);
            this->addChild(overLabel);
        } else {
            
            AXLOG("Error initializing EndScreen: Background or overLabel creation failed.");
            return false;
        }
        return true;
    }

    
    void setLabelText(const std::string& text) {
        if (overLabel) {
            overLabel->setString(text);  
        } else {
            AXLOG("Error: overLabel is null in setLabelText.");
        }
    }

    
    static EndScreen* create() {
        EndScreen* ret = new (std::nothrow) EndScreen();
        if (ret && ret->init()) {
            ret->autorelease();  
            return ret;
        } else {
            delete ret; 
            return nullptr;
        }
    }
};



class MainScene : public ax::Scene {
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
    
public:
    bool init() override;
    void update(float delta) override;
    
    // touch
    void onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);
    
    // mouse
    void onMouseDown(ax::Event* event);
    void onMouseUp(ax::Event* event);
    void onMouseMove(ax::Event* event);
    void onMouseScroll(ax::Event* event);
    
    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);
    
    // a selector callback
    void menuCloseCallback(ax::Object* sender);
    
    
    
private:
    ax::Label* backLabel = nullptr;
    GameState _gameState = GameState::init;
    
    
    
public:
    GameBoard gameBoard;
    int placeDisc(int);
    virtual int placeDiscAt(ax::Vec2);
    ax::Vec2 getBoardCoords(int, int);
    ax::Sprite* getDisc(TURN, bool);
    void signalGameOver();
    void signalGameOver(bool forfeited);
    ax::Rect getRect(TURN);
    void glowUp(int, int, TURN, const std::function<void()>&);
    
    void showGameOverScreen(TURN);
    
    void signalDraw();
    
    virtual int askBot() {};
    
    virtual void onBackPressed(){};

};

#endif  // __MAIN_SCENE_H__
