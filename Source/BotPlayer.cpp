#include "BotPlayer.h"
#include "constants.h"
#include <random>
#include <cmath>
#include <thread>


const auto USER_TURN = RED;


int checkAlignment(GameBoard& gb, int row, int col, int dRow, int dCol) {
    int piece = gb.game_board_map[col][row];
    int alignmentCount = 1;
    
    for (int i = 1; i < 4; i++) {
        int newRow = row + i * dRow;
        int newCol = col + i * dCol;
        if (newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 7 && gb.game_board_map[newCol][newRow] == piece) {
            alignmentCount++;
        } else {
            break;
        }
    }

    for (int i = 1; i < 4; i++) {
        int newRow = row - i * dRow;
        int newCol = col - i * dCol;
        if (newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 7 && gb.game_board_map[newCol][newRow] == piece) {
            alignmentCount++;
        } else {
            break;
        }
    }

    if (alignmentCount >= 2) {
        return std::pow(100, alignmentCount - 1) * -1 * piece;
                                                // ^ this should be variable based on user turn
    }

    return 0;
}


int evaluateBoard(GameBoard& gb) {
    int score = 0;
    
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            if (gb.game_board_map[col][row] != 0) {
                score += checkAlignment(gb, row, col, 1, 0);  // EW
                score += checkAlignment(gb, row, col, 0, 1);  // NS
                score += checkAlignment(gb, row, col, 1, 1);  // NW
                score += checkAlignment(gb, row, col, 1, -1); // NE
            }
        }
    }

    return score;
}


int minimax(GameBoard& gb, int alpha, int beta, int depth, bool isMaximizing) {
    if (depth == 0 || gb.isGameOver()) {
        return evaluateBoard(gb);
    }

    int best_score = isMaximizing ? INT_MIN : INT_MAX;

    for (int col = 0; col < 7; col++) {
        if (!gb.isValidMove(col)) {
            continue;
        }

        gb.makeMoveAndGetCol(col);
        int miniMaxScore = minimax(gb, alpha, beta, depth - 1, !isMaximizing);
        gb.undoMove();

        if (isMaximizing) {
            best_score = std::max(miniMaxScore, best_score);
            alpha = std::max(alpha, best_score);
        } else {
            best_score = std::min(miniMaxScore, best_score);
            beta = std::min(beta, best_score);
        }

        if (alpha >= beta) {
            break;
        }
    }

    return best_score;
}

int BotPlayer::askBot() {
    AXLOG("Asking bot...");
    int bestMove = -1;
    int bestScore = INT_MIN;
    for(int col = 0; col < 7; col++){
        if(!gameBoard.isValidMove(col)) {
            continue;
        }
        gameBoard.makeMoveAndGetCol(col);
        auto miniMaxScore = minimax(gameBoard, INT_MIN, INT_MAX, 7, false);
        if (miniMaxScore > bestScore){
            bestMove = col;
            bestScore = miniMaxScore;
        }
        gameBoard.undoMove();
    }
    return bestMove;
}

void BotPlayer::onMouseDown(ax::Event* event){
    if(gameBoard.getCurrentTurn() == USER_TURN) {
        MainScene::onMouseDown(event);
        
        if(!gameBoard.isGameOver()){
            std::thread botThread([this]() {
                placeDisc(askBot());
            });
            botThread.detach();
        }
    }
}
