//
//  GameBoard.cpp
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 07/01/25.
//
#include "GameBoard.h"
#include<iostream>
#include "axmol.h"

const std::string GameBoard::stringFromTurn(TURN t) {
    switch(t){
        case RED:
            return "Red";
        case YELLOW:
            return "Yellow";
        case NONE:
            return "None";
    };
}

bool GameBoard::isRedTurn() {
    return currentTurn == RED;
}


std::array<std::pair<int, int>, 4> GameBoard::getWinningCoins() {
    return winningCoins;
}

move GameBoard::getLastMove() {
    return movesDone.top();
}

TURN GameBoard::swapTurn(TURN turn) {
    return static_cast<TURN>(static_cast<int>(turn) * -1);
}

void GameBoard::changeTurn(){
    currentTurn = swapTurn(currentTurn);
}

bool GameBoard::checkRowFull(int row){
    return row_occupancies[row] >= 5;
}

bool GameBoard::isValidMove(int row){
    return !(checkRowFull(row) || gameOver);
}

int GameBoard::makeMoveAndGetCol(int row){
    if(!isValidMove(row)){
        return -1;
    }
    movesDone.push({row, currentTurn});
    game_board_map[row][++row_occupancies[row]] = static_cast<int>(currentTurn);
    changeTurn();
    validateBoard();
    return row_occupancies[row];
}

void GameBoard::undoMove() {
    auto lastMove = movesDone.top();
    movesDone.pop();
    changeTurn();
    
    game_board_map[lastMove.row][row_occupancies[lastMove.row]] = 0;
    gameOver = false;
    row_occupancies[lastMove.row]--;
}

TURN GameBoard::getCurrentTurn() {
    return currentTurn;
}

bool GameBoard::isGameOver(){
    return gameOver;
}

void GameBoard::validateBoard() {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int player = game_board_map[col][row];
            if (player != 0 &&
                player == game_board_map[col + 1][row] &&
                player == game_board_map[col + 2][row] &&
                player == game_board_map[col + 3][row]) {
                gameOver = true;
                winningCoins[0] = std::make_pair(col, row);
                winningCoins[1] = std::make_pair(col+1, row);
                winningCoins[2] = std::make_pair(col+2, row);
                winningCoins[3] = std::make_pair(col+3, row);
                return;
            }
        }
    }

    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            int player = game_board_map[col][row];
            if (player != 0 &&
                player == game_board_map[col][row + 1] &&
                player == game_board_map[col][row + 2] &&
                player == game_board_map[col][row + 3]) {
                gameOver = true;
                winningCoins[0] = std::make_pair(col, row);
                winningCoins[1] = std::make_pair(col, row+1);
                winningCoins[2] = std::make_pair(col, row+2);
                winningCoins[3] = std::make_pair(col, row+3);
                return;
            }
        }
    }

    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 3; ++row) {
            int player = game_board_map[col][row];
            if (player != 0 &&
                player == game_board_map[col + 1][row + 1] &&
                player == game_board_map[col + 2][row + 2] &&
                player == game_board_map[col + 3][row + 3]) {
                gameOver = true;
                winningCoins[0] = std::make_pair(col, row);
                winningCoins[1] = std::make_pair(col+1, row+1);
                winningCoins[2] = std::make_pair(col+2, row+2);
                winningCoins[3] = std::make_pair(col+3, row+3);
                return;
            }
        }
    }


    for (int col = 3; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            int player = game_board_map[col][row];
            if (player != 0 &&
                player == game_board_map[col - 1][row + 1] &&
                player == game_board_map[col - 2][row + 2] &&
                player == game_board_map[col - 3][row + 3]) {
                gameOver = true;
                winningCoins[0] = std::make_pair(col, row);
                winningCoins[1] = std::make_pair(col-1, row+1);
                winningCoins[2] = std::make_pair(col-2, row+2);
                winningCoins[3] = std::make_pair(col-3, row+3);
                return;
            }
        }
    }
    
    gameOver = isDraw();
}

bool GameBoard::isDraw() {
    return movesDone.size() == 6*7;
}
