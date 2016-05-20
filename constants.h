#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// 15 x 15 board
const int BOARD_SIZE = 15;

const int EMPTY_GRID = 0;
const int BLACK_GRID = 1;
const int WHITE_GRID = 2;

const int BLACK_PLAYER = 0;
const int WHITE_PLAYER = 1;

inline int get_grid_from_player(int player) {
	return player == BLACK_PLAYER ? BLACK_GRID : WHITE_GRID;
}

#endif
