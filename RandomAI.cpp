#include "RandomAI.h"

#include <ctime>
#include <cstring>
#include <cassert>

RandomAI::RandomAI() {
	seed = (int)time(NULL);
	if (seed < 0) seed = -seed;
	if (seed <= 0) seed = 1;  // -2147483648
	
	memset(board, 0, sizeof(board));
}

RandomAI::~RandomAI() {
	
}

void RandomAI::get_next_seed() {
	seed = seed * 16807LL % 2147483647LL;
}

void RandomAI::init(int player) {
	this->player = player;
	this->my_grid = get_grid_from_player(player);
	this->oppo_grid = get_grid_from_player(player ^ 1);
}

void RandomAI::opponent_step(int x, int y, int player) {
	board[x][y] = oppo_grid;
}

bool RandomAI::get_step(int &x, int &y) {
	int cnt = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			cnt += board[i][j] == 0;
		}
	}
	if (cnt == 0) {
		return false;
	}
	get_next_seed();
	int k = seed % cnt + 1;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == 0) {
				if (--k == 0) {
					x = i;
					y = j;
					board[x][y] = my_grid;
					return true;
				}
			}
		}
	}
	
	assert(false);
}
