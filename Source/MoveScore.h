//
//  MoveScore.h
//  Connect4
//
//  Created by Hrishik Sai Bojnal on 08/01/25.
//

#ifndef __MOVE_SCORE_H__
#define __MOVE_SCORE_H__

struct MoveScore {
    int move;
    int score;
    
    bool operator< (const MoveScore& other) const {
        return score < other.score;
    }
};

#endif
