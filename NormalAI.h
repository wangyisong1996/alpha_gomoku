#ifndef __NORMAL_AI_H__
#define __NORMAL_AI_H__

#include "RNG.h"
#include "Constants.h"
#include "BasicAI.h"

class NormalAI : public BasicAI {
public:
    NormalAI();
    ~NormalAI();

    void init(int player);
    void opponent_step(int x, int y, int player);
    bool get_step(int &x, int &y);

private:
	int board[BOARD_SIZE][BOARD_SIZE];
	int player;
	int round;
	int my_grid;
	int oppo_grid;
	int last_x, last_y;
};

#endif  // NORMAL_AI_H
