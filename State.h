#ifndef STATE_H
#define STATE_H

#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>
#include "Constants.h"
#include "Point.h"

class State {
public:
	State(const int board_[][BOARD_SIZE], const int player_, const int last_x, const int last_y);
	~State();

	bool can_move() const { return winner == EMPTY_GRID; }
	bool can_move(const Point& p) const { return can_move() && !occupy[p.x][p.y]; }

	std::vector<Point> all_move() const;

	bool move(const Point& p);

	// return the winner after taking move p
	int check_move(const Point& p) const;
	int get_winner() const { return this->winner; }
	int get_color() const { return this->color; }
	int find_winner();

	double reward(const int color) {
		if (color == winner)
			return 1.0;
		else if (color == TIE)
			return 0.5;
		else
			return 0.0;
	}

	bool tie() const;

	friend std::ostream& operator<<(std::ostream& os, const State& s) {
		static char symbol[] = {'.', 'X', 'O'};
		for (int i = 0; i < BOARD_SIZE; ++i) {
			for (int j = 0; j < BOARD_SIZE; ++j) {
				os << symbol[s.board[i][j]] << " ";
			}
			os << std::endl;
		}
		return os;
	}

private:
	static const int TIE;
	int color;
	int board[BOARD_SIZE][BOARD_SIZE];
	bool occupy[BOARD_SIZE][BOARD_SIZE];

	int lastX, lastY;
	int winner;
};

// friend std::ostream& operator<<(std::ostream& os, const State& s) {
// 	for (int i = 0; i < BOARD_SIZE; ++i) {
// 		for (int j = 0; j < BOARD_SIZE; ++j) {
// 			os << s.board[i][j] << " ";
// 		}
// 		os << std::endl;
// 	}
// 	return os;
// }

#endif  // STATE_H