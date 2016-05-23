#include "State.h"
#include <cstring>
#include <vector>
using namespace std;

const int State::TIE = 4;

State::State(const int board_[][BOARD_SIZE], const int player_,
             const int last_x, const int last_y)
    : color(player_ + 1), winner(EMPTY_GRID), lastX(last_x), lastY(last_y) {
    memcpy(board, board_, sizeof(board));
    memset(occupy, 0, sizeof(occupy));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            // cout << board_[i][j] << " ";
            if (board_[i][j] != EMPTY_GRID) occupy[i][j] = true;
        }
        // cout << endl;
    }
}

State::~State() {}

bool State::tie() const {
    if (get_winner() != EMPTY_GRID) return false;
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (!occupy[i][j]) return false;
    return true;
}

vector<Point> State::all_move() const {
    vector<Point> move;
    move.reserve(BOARD_SIZE * BOARD_SIZE);
    if (winner == EMPTY_GRID) {
        for (int i = 0; i < BOARD_SIZE; ++i)
            for (int j = 0; j < BOARD_SIZE; ++j) {
                // if (!occupy[i][j])
                // move.push_back(Point(i, j));
                // else
                // cout << "(" << i << "," << j << ")" << endl;
                if (board[i][j] == EMPTY_GRID) move.push_back(Point(i, j));
            }
    }

    return move;
}

bool State::move(const Point& p) {
    if (!can_move(p)) return false;
    board[p.x][p.y] = color;
    occupy[p.x][p.y] = true;
    lastX = p.x, lastY = p.y;
    color = (color == BLACK_GRID) ? WHITE_GRID : BLACK_GRID;
    find_winner();
    return true;
}

// TODO: implement
int State::check_move(const Point& p) const {
    // int player = EMPTY_GRID;
}

int State::find_winner() {
    if (lastX < 0 || lastY < 0) return winner = EMPTY_GRID;

    int i, j, col, row;
    int player = board[lastX][lastY];

    i = 0, j = 0;
    for (col = lastY - 1; col >= 0 && board[lastX][col] == player; --col) ++i;
    for (col = lastY + 1; col < BOARD_SIZE && board[lastX][col] == player;
         ++col)
        ++j;
    if (i + j + 1 >= 5) return winner = player;

    i = 0, j = 0;
    for (row = lastX - 1; row >= 0 && board[row][lastY] == player; --row) ++i;
    for (row = lastX + 1; row < BOARD_SIZE && board[row][lastY] == player;
         ++row)
        ++j;
    if (i + j + 1 >= 5) return winner = player;

    i = 0, j = 0;
    for (col = lastY - 1, row = lastX - 1;
         col >= 0 && row >= 0 && board[row][col] == player; --col, --row)
        ++i;
    for (col = lastY + 1, row = lastX + 1;
         col < BOARD_SIZE && row < BOARD_SIZE && board[row][col] == player;
         ++col, ++row)
        ++j;
    if (i + j + 1 >= 5) return winner = player;

    i = 0, j = 0;
    for (col = lastY - 1, row = lastX + 1;
         col >= 0 && row < BOARD_SIZE && board[row][col] == player;
         --col, ++row)
        ++i;
    for (col = lastY + 1, row = lastX - 1;
         col < BOARD_SIZE && row > 0 && board[row][col] == player; ++col, --row)
        ++j;
    if (i + j + 1 >= 5) return winner = player; 

    // bool tie = true;
    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //     for (int j = 0; j < BOARD_SIZE; ++j) {
    //         if (board[i][j] == EMPTY_GRID) {
    //             tie = false;
    //             break;
    //         }
    //     }
    //     if (!tie) break;
    // }

    return winner = tie() ? TIE : EMPTY_GRID;
}