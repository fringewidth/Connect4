//
//  GameBoard.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 07/01/25.
//
#ifndef __GAME_BOARD_H__
#define __GAME_BOARD_H__
#include<stack>
#include<array>
#include "MoveScore.h"


enum TURN {
    RED = 1,
    YELLOW = -1,
    NONE = 0
};

struct move {
    int row;
    TURN turn;
};

class GameBoard {
private:
    std::stack<move> movesDone;
    std::array<int, 7> row_occupancies = {-1, -1, -1, -1, -1, -1, -1};
    TURN currentTurn = RED;
    
    bool checkRowFull(int);
    void validateBoard();
    void changeTurn ();
        
    bool gameOver = false;
    
public:
    std::array<std::array<int, 6>, 7> game_board_map{0};
    bool isRedTurn();
    bool isValidMove(int);
    int makeMoveAndGetCol(int);
    void undoMove();
    TURN swapTurn(TURN);
    TURN getCurrentTurn();
    bool isGameOver();
    move getLastMove();

};

#endif // __GAME_BOARD_H__
