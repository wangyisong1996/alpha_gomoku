#ifndef NORMAL_AI_H
#define NORMAL_AI_H

#include "Rng.h"
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
	int my_grid;
	int oppo_grid;
	int last_x, last_y;
};

#endif  // NORMAL_AI_H
