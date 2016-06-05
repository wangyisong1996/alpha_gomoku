#include "State.h"
#include <cstring>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

const int State::TIE = 4;
const int State::px[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int State::py[8] = {0, 1, 1, 1, 0, -1, -1, -1};

State::State(const int board_[][BOARD_SIZE], const int player,
             const int lastX, const int lastY)
    : color(player + 1), winner(EMPTY_GRID), lastX(lastX), lastY(lastY) {
    memcpy(board, board_, sizeof(board));
    memset(occupy, 0, sizeof(occupy));
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board_[i][j] != EMPTY_GRID) occupy[i][j] = true;
        }
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
    move.reserve(10);
    if (winner == EMPTY_GRID) {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == EMPTY_GRID) {
                    for (int k = 0; k < 8; ++k) {
                        int tx = i + px[k];
                        int ty = j + py[k];
                        if (tx < 0 || tx > 14 || ty < 0 || ty > 14) {
                            continue;
                        } else if (board[tx][ty] != EMPTY_GRID) {
                            move.push_back(Point(i, j));
                            break;
                        }
                    }
                }
            }
        }
    }
    // static char symbol[] = {'.', 'X', 'O'};
    //     for (int i = 0; i < BOARD_SIZE; ++i) {
    //         for (int j = 0; j < BOARD_SIZE; ++j) {
    //             cout << symbol[board[i][j]] << " ";
    //         }
    //         cout << std::endl;
    //     }

    // if (move.empty()) move.push_back(Point(BOARD_SIZE / 2, BOARD_SIZE / 2));
    // for (auto&& i : move) {
    //     cout << i << endl;
    // }
    // cout << "---------------" << endl;
    // getchar();
    // random_shuffle(move.begin(), move.end());
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