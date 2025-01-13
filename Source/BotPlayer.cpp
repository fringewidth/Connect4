#include "BotPlayer.h"
#include "constants.h"
#include <random>


const auto USER_TURN = RED;

void BotPlayer::onMouseDown(ax::Event* event){
    if(gameBoard.getCurrentTurn() == USER_TURN) {
        MainScene::onMouseDown(event);
        while(!gameBoard.isGameOver() && placeDisc(askBot()) == DISC_UNPLACED)
            ; // ask bot to try again until disc is placed or game is over.
    }
}

int BotPlayer::askBot() {
    return rand() % 7;
}
