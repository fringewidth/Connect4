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
    void onMouseDown(ax::Event* event) override;
    virtual int askBot();    
};

#endif
