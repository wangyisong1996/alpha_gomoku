#include "NormalAI.h"
#include "State.h"
#include "UCT.h"
#include "Constants.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

NormalAI::NormalAI() {
	memset(board, 0, sizeof(board));
	last_x = -1, last_y = -1;	
}

NormalAI::~NormalAI() {

}

void NormalAI::init(int player) {
	this->player = player;
	this->my_grid = get_grid_from_player(player);
	this->oppo_grid = get_grid_from_player(player ^ 1);
}

void NormalAI::opponent_step(int x, int y, int player) {
	board[x][y] = oppo_grid;
	last_x = x, last_y = y;
	// std::cout << x << ", " << y << std::endl;
}

bool NormalAI::get_step(int &x, int &y) {
	State root_state(this->board, player, last_x, last_y);
	tree search_tree(root_state);
	Point p = search_tree.uct();
	x = p.x, y = p.y;
	last_x = x, last_y = y;
	this->board[x][y] = my_grid;
	return true;
}