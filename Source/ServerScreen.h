#ifndef __SERVER_SCREEN_H__
#define __SERVER_SCREEN_H__

#include <axmol.h>

class ServerScreen : public ax::Scene {
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
    int messageDotCount = 0;
    void updateText(ax::Label*);
    
protected:
    std::string_view serverLoadingMessage;
    std::string_view errorMessage;
    ax::Label* centerLabel;
    bool connectionFailed = false;

    
public:
    ServerScreen();

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

};

#endif
