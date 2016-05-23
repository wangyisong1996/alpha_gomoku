#include "Graphics.h"
#include "Constants.h"
#include "math.h"
#include "BasicAI.h"

#include "NormalAI.h"

#include <GLFW/glfw3.h>

#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <thread>

#define debug(...) {fprintf(stderr, __VA_ARGS__); fflush(stderr);}

Graphics *g = NULL;

const int GAME_NOT_STARTED = 0;
const int GAME_OVER = 1;
const int BLACK_TURN = 2;
const int WHITE_TURN = 3;

const double AI_WAIT_TIME = 0.2;

int game_state = GAME_NOT_STARTED;

static bool last_key_state[350] = {0};
static bool now_key_state[350] = {0};

void check_key_states() {
	for (int i = 32; i < 348; i++) {
		last_key_state[i] = now_key_state[i];
		now_key_state[i] = g->get_key_state(i) == GLFW_PRESS;
	}
}

bool check_key(int key) {
	return !last_key_state[key] && now_key_state[key];
}

double t_now;

bool is_human[2];

int cursor_pos_x[2], cursor_pos_y[2];

int board[BOARD_SIZE][BOARD_SIZE];

double t_turn;

BasicAI *AIs[2];

std::thread *AI_thread;
bool AI_done;
int AI_x, AI_y;

int last_player_x, last_player_y;

void switch_player(int player);

void start_game(bool is_black_human, bool is_white_human) {
	is_human[BLACK_PLAYER] = is_black_human;
	is_human[WHITE_PLAYER] = is_white_human;

	for (int i = 0; i < 2; i++) {
		cursor_pos_x[i] = cursor_pos_y[i] = 7;

		if (!is_human[i]) {
			AIs[i] = new NormalAI();
			AIs[i]->init(i);
		}
	}

	last_player_x = last_player_y = -1;

	memset(board, 0, sizeof(board));

	// for (int i = 0; i < BOARD_SIZE; i++) {
	// 	for (int j = 0; j < BOARD_SIZE; j++) {
	// 		board[i][j] = rand() % 3;
	// 	}
	// }

	switch_player(BLACK_PLAYER);
}

void draw_frame_fixed(double x, double y, double size, float r, float g, float b) {
	size *= 0.5;
	::g->draw_line(x - size, y - size, x + size, y - size, r, g, b);
	::g->draw_line(x + size, y - size, x + size, y + size, r, g, b);
	::g->draw_line(x + size, y + size, x - size, y + size, r, g, b);
	::g->draw_line(x - size, y + size, x - size, y - size, r, g, b);
}

void draw_frame(double x, double y, double size, float r, float g, float b) {
	size *= 0.25;

	::g->draw_line(x + size * 2, y + size, x + size * 2, y + size * 2, r, g, b);
	::g->draw_line(x + size * 2, y + size * 2, x + size, y + size * 2, r, g, b);

	::g->draw_line(x - size * 2, y + size, x - size * 2, y + size * 2, r, g, b);
	::g->draw_line(x - size * 2, y + size * 2, x - size, y + size * 2, r, g, b);

	::g->draw_line(x + size * 2, y - size, x + size * 2, y - size * 2, r, g, b);
	::g->draw_line(x + size * 2, y - size * 2, x + size, y - size * 2, r, g, b);

	::g->draw_line(x - size * 2, y - size, x - size * 2, y - size * 2, r, g, b);
	::g->draw_line(x - size * 2, y - size * 2, x - size, y - size * 2, r, g, b);
}

int winner;
int winner_x0, winner_y0, winner_x1, winner_y1;

void render_game() {
	g->draw_line(v2d(1, 0), v2d(1, 1));

	double tmp = 1.0 / (BOARD_SIZE + 1);

	for (int i = 1; i <= BOARD_SIZE; i++) {
		double t = i * tmp;
		g->draw_line(v2d(t, 0), v2d(t, 1), 0.5f, 0.5f, 0.5f);
		g->draw_line(v2d(0, t), v2d(1, t), 0.5f, 0.5f, 0.5f);
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		double y = (BOARD_SIZE - i) * tmp;
		for (int j = 0; j < BOARD_SIZE; j++) {
			double x = (j + 1) * tmp;
			double r = tmp / 2 * 0.9;
			if (board[i][j] == BLACK_GRID) {
				g->draw_circle(v2d(x, y), r, 0.0f, 0.0f, 0.0f);
			} else if (board[i][j] == WHITE_GRID) {
				g->draw_circle(v2d(x, y), r, 1.0f, 1.0f, 1.0f);
			}
		}
	}

	int id = game_state == BLACK_TURN ? BLACK_PLAYER : WHITE_PLAYER;

	if (game_state == GAME_OVER) {
		id = winner;
	}

	double y = (BOARD_SIZE - cursor_pos_x[id ^ 1]) * tmp;
	double x = (cursor_pos_y[id ^ 1] + 1) * tmp;
	draw_frame_fixed(x, y, tmp * 0.9, 0.7f, 0.7f, 0.7f);

	y = (BOARD_SIZE - cursor_pos_x[id]) * tmp;
	x = (cursor_pos_y[id] + 1) * tmp;
	draw_frame(x, y, tmp * (0.9 + 0.1 * sin((t_now - t_turn) * 2 * PI / 0.5)), 0.1f, 0.1f, 0.1f);
}

void AI_thread_func(BasicAI *AI, int oppo_x, int oppo_y, int oppo_player) {
	if (oppo_x != -1) {
		AI->opponent_step(oppo_x, oppo_y, oppo_player);
	}
	if (!AI->get_step(AI_x, AI_y)) {
		AI_x = -1;
		AI_y = -1;
	}
	AI_done = true;
}

void switch_player(int player) {
	game_state = player == BLACK_PLAYER ? BLACK_TURN : WHITE_TURN;
	t_turn = t_now;
	if (!is_human[player]) {
		AI_done = false;
		AI_thread = new std::thread(AI_thread_func, AIs[player], last_player_x, last_player_y, player ^ 1);
		AI_thread->detach();
	}
}

#define INF 0x3f3f3f3f

bool check_wins(int player) {
	int val = player == BLACK_PLAYER ? BLACK_GRID : WHITE_GRID;

	const int dx[4] = {1, 1, 0, -1};
	const int dy[4] = {0, 1, 1, 1};

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] != val) continue;
			for (int _ = 0 ; _ < 4; _++) {
				int x1 = i + dx[_] * 4;
				int y1 = j + dy[_] * 4;
				if (x1 < 0 || x1 >= BOARD_SIZE || y1 < 0 || y1 >= BOARD_SIZE) continue;
				int k;
				for (k = 1; k <= 4; k++) {
					int x = i + dx[_] * k;
					int y = j + dy[_] * k;
					if (board[x][y] != val) break;
				}
				if (k > 4) {
					winner = player;
					winner_x0 = i;
					winner_y0 = j;
					winner_x1 = x1;
					winner_y1 = y1;
					return true;
				}
			}
		}
	}

	return false;
}

bool check_game_over() {
	return check_wins(BLACK_PLAYER) || check_wins(WHITE_PLAYER);
}

void do_move(int player, int x, int y) {
	debug("move %d %d %d\n", player, x, y);

	if (board[x][y] != EMPTY_GRID) {
		// invalid move ...
		game_state = GAME_OVER;
		winner_x0 = -1;
		return;
	}

	board[x][y] = player == BLACK_PLAYER ? BLACK_GRID : WHITE_GRID;

	last_player_x = x;
	last_player_y = y;

	cursor_pos_x[player] = x;
	cursor_pos_y[player] = y;

	if (check_game_over()) {
		game_state = GAME_OVER;
	} else {
		switch_player(player ^ 1);
	}
}

void process_game_not_started() {
	if (check_key(GLFW_KEY_1)) {
		// human vs. human
		start_game(true, true);
	} else if (check_key(GLFW_KEY_2)) {
		// human vs. AI
		start_game(true, false);
	} else if (check_key(GLFW_KEY_3)) {
		// AI vs. human
		start_game(false, true);
	} else if (check_key(GLFW_KEY_4)) {
		// AI vs. AI
		start_game(false, false);
	}
}

void process_black_turn() {
	render_game();

	if (is_human[BLACK_PLAYER]) {
		if (check_key(GLFW_KEY_SPACE)) {
			int x = cursor_pos_x[BLACK_PLAYER];
			int y = cursor_pos_y[BLACK_PLAYER];
			if (board[x][y] == 0) {
				do_move(BLACK_PLAYER, x, y);
			}
		} else {
			if (check_key(GLFW_KEY_RIGHT)) {
				++cursor_pos_y[BLACK_PLAYER];
			} else if (check_key(GLFW_KEY_LEFT)) {
				--cursor_pos_y[BLACK_PLAYER];
			} else if (check_key(GLFW_KEY_UP)) {
				--cursor_pos_x[BLACK_PLAYER];
			} else if (check_key(GLFW_KEY_DOWN)) {
				++cursor_pos_x[BLACK_PLAYER];
			}
			cursor_pos_x[BLACK_PLAYER] = clamp(cursor_pos_x[BLACK_PLAYER], 0, BOARD_SIZE - 1);
			cursor_pos_y[BLACK_PLAYER] = clamp(cursor_pos_y[BLACK_PLAYER], 0, BOARD_SIZE - 1);
		}
	} else {
		if (t_now - t_turn >= AI_WAIT_TIME && AI_done) {
			delete AI_thread;
			AI_thread = NULL;
			do_move(BLACK_PLAYER, AI_x, AI_y);
		}
	}
}

void process_white_turn() {
	render_game();

	if (is_human[WHITE_PLAYER]) {
		if (check_key(GLFW_KEY_SPACE)) {
			int x = cursor_pos_x[WHITE_PLAYER];
			int y = cursor_pos_y[WHITE_PLAYER];
			if (board[x][y] == 0) {
				do_move(WHITE_PLAYER, x, y);
			}
		} else {
			if (check_key(GLFW_KEY_RIGHT)) {
				++cursor_pos_y[WHITE_PLAYER];
			} else if (check_key(GLFW_KEY_LEFT)) {
				--cursor_pos_y[WHITE_PLAYER];
			} else if (check_key(GLFW_KEY_UP)) {
				--cursor_pos_x[WHITE_PLAYER];
			} else if (check_key(GLFW_KEY_DOWN)) {
				++cursor_pos_x[WHITE_PLAYER];
			}
			cursor_pos_x[WHITE_PLAYER] = clamp(cursor_pos_x[WHITE_PLAYER], 0, BOARD_SIZE - 1);
			cursor_pos_y[WHITE_PLAYER] = clamp(cursor_pos_y[WHITE_PLAYER], 0, BOARD_SIZE - 1);
		}
	} else {
		if (t_now - t_turn >= AI_WAIT_TIME && AI_done) {
			delete AI_thread;
			AI_thread = NULL;
			do_move(WHITE_PLAYER, AI_x, AI_y);
		}
	}
}

void process_game_over() {
	render_game();

	if (winner_x0 >= 0) {
		double tmp = 1.0 / (BOARD_SIZE + 1);

		double y0 = (BOARD_SIZE - winner_x0) * tmp;
		double x0 = (winner_y0 + 1) * tmp;

		double y1 = (BOARD_SIZE - winner_x1) * tmp;
		double x1 = (winner_y1 + 1) * tmp;

		g->draw_line(x0, y0, x1, y1, 0.0f, 1.0f, 0.0f);

	}

	if (check_key(GLFW_KEY_ESCAPE)) {
		game_state = GAME_NOT_STARTED;
	}
}

bool loop_func() {

	// t_now = 10.0 * (double)clock() / (double)CLOCKS_PER_SEC;
    t_now = glfwGetTime();

	check_key_states();

	if (game_state == GAME_NOT_STARTED) {
		process_game_not_started();
	} else if (game_state == BLACK_TURN) {
		process_black_turn();
	} else if (game_state == WHITE_TURN) {
		process_white_turn();
	} else if (game_state == GAME_OVER) {
		process_game_over();
	} else {
		// WTF??
	}

	return true;
}

int main() {
	g = new Graphics("Alpha Gomoku", 800, 600);

	g->main_loop(loop_func);

	delete g;
}
