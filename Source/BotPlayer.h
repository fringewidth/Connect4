//
//  TwoPlayer.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 07/01/25.
//
#ifndef __BOT_PLAYER_H__
#define __BOT_PLAYER_H__

#include "axmol.h"
#include "MainScene.h"
#include "MoveScore.h"

class BotPlayer : public MainScene {
    virtual int placeDiscAt(ax::Vec2) override;
    virtual int askBot() override;
    std::mutex botMutex;
    std::mutex turnMutex;
    bool isBotActive = false;
    
public:
    bool myTurn = true;

};

#endif
