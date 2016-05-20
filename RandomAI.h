#ifndef __RANDOM_AI_H__
#define __RANDOM_AI_H__

#include "BasicAI.h"
#include "Constants.h"

class RandomAI : public BasicAI {
	public:
	RandomAI();
	~RandomAI();
	
	void init(int player);
	void opponent_step(int x, int y, int player);
	bool get_step(int &x, int &y);
	
	private:
	int board[BOARD_SIZE][BOARD_SIZE];
	int seed;
	int player;
	int my_grid;
	int oppo_grid;
	
	void get_next_seed();
};

#endif
