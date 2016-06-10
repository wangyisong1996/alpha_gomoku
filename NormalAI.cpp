#include "NormalAI.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Constants.h"
#include "State.h"
#include "UCT.h"

NormalAI::NormalAI() : round(0) {
    memset(board, 0, sizeof(board));
    last_x = -1, last_y = -1;
}

NormalAI::~NormalAI() {}

void NormalAI::init(int player) {
    this->player = player;
    this->my_grid = get_grid_from_player(player);
    this->oppo_grid = get_grid_from_player(player ^ 1);
}

void NormalAI::opponent_step(int x, int y, int player) {
    board[x][y] = oppo_grid;
    last_x = x;
    last_y = y;
    ++round;
}

bool NormalAI::get_step(int &x, int &y) {
    Point p = Point(BOARD_SIZE / 2, BOARD_SIZE / 2);
    if (round != 0) {
        State root_state(this->board, player, last_x, last_y);
        Tree search_tree(root_state, round);
        p = search_tree.UCT();
    }
    last_x = x = p.x;
    last_y = y = p.y;
    ++round;
    this->board[x][y] = my_grid;
    return true;
}