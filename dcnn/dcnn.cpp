#include "dcnn.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include "Constants.h"
#include "Point.h"
#include "prettyprint.hpp"

#define CPU_ONLY 1
#include <caffe/caffe.hpp>

using namespace caffe;

const int CNN::channel = 7;
std::shared_ptr<caffe::Net<float> > CNN::net;

CNN::CNN(const int color) : color(color) {
    if (this->net) return;

    const char *model_file = "CNN.prototxt";
    const char *trained_file = "Model.caffemodel";

    Caffe::set_mode(Caffe::CPU);

    // Load the network
    net.reset(new caffe::Net<float>(model_file, TEST));
    net->CopyTrainedLayersFrom(trained_file);

    std::cout << "CNN Initialized." << std::endl;
}

CNN::~CNN() {}

int CNN::connect_num(const int board[][BOARD_SIZE], const int x,
                     const int y) const {
    int i, j, col, row;
    int player = board[x][y];
    bool flag = false;

    i = 0, j = 0;
    for (col = y - 1; col >= 0 && board[x][col] == player; --col) ++i;
    for (col = y + 1; col < BOARD_SIZE && board[x][col] == player; ++col) ++j;
    if (i + j + 1 == 4) {
        return 4;
    } else if (i + j + 1 == 3) {
        flag = true;
    }

    i = 0, j = 0;
    for (row = x - 1; row >= 0 && board[row][y] == player; --row) ++i;
    for (row = x + 1; row < BOARD_SIZE && board[row][y] == player; ++row) ++j;
    if (i + j + 1 == 4) {
        return 4;
    } else if (i + j + 1 == 3) {
        flag = true;
    }

    i = 0, j = 0;
    for (col = y - 1, row = x - 1;
         col >= 0 && row >= 0 && board[row][col] == player; --col, --row)
        ++i;
    for (col = y + 1, row = x + 1;
         col < BOARD_SIZE && row < BOARD_SIZE && board[row][col] == player;
         ++col, ++row)
        ++j;
    if (i + j + 1 == 4) {
        return 4;
    } else if (i + j + 1 == 3) {
        flag = true;
    }

    i = 0, j = 0;
    for (col = y - 1, row = x + 1;
         col >= 0 && row < BOARD_SIZE && board[row][col] == player;
         --col, ++row)
        ++i;
    for (col = y + 1, row = x - 1;
         col < BOARD_SIZE && row > 0 && board[row][col] == player; ++col, --row)
        ++j;
    if (i + j + 1 == 4) {
        return 4;
    } else if (i + j + 1 == 3) {
        flag = true;
    }

    if (flag) {
        return 3;
    } else {
        return 0;
    }
}

vector<Point> CNN::get_moves(const State &s) const {
    static const int size = BOARD_SIZE;
    static const int size2 = size * size;
    static const int dsize = channel * size2;

    float *data = new float[dsize];
    memset(data, 0, sizeof(data));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int p = size * i + j;
            if (s.board[i][j] == EMPTY_GRID) {
                data[0 * size2 + p] = 1.0;
            } else if (s.board[i][j] == color) {
                data[1 * size2 + p] = 1.0;
                int num = connect_num(s.board, i, j);
                if (num == 3) {
                    data[3 * size2 + p] = 1.0;
                } else if (num == 4) {
                    data[5 * size2 + p] = 1.0;
                }
            } else {
                data[2 * size2 + p] = 1.0;
                int num = connect_num(s.board, i, j);
                if (num == 3) {
                    data[4 * size2 + p] = 1.0;
                } else if (num == 4) {
                    data[6 * size2 + p] = 1.0;
                }
            }
        }
    }

    Blob<float> *blob = new Blob<float>(1, 7, size, size);
    blob->set_cpu_data(data);
    std::vector<Blob<float> *> bottom;
    bottom.push_back(blob);

    net->Forward(bottom);
    Blob<float>* output_layer = net->output_blobs()[0];

    const float* begin = output_layer->cpu_data();
    const float* end = begin + output_layer->channels();
    std::vector<float> rr(begin, end);

    std::vector<std::pair<int, float> > result;
    for (int i = 0; i < size2; i++) {
        result.push_back(std::make_pair(i, rr[i]));
    }
    std::cout << result << std::endl;

    sort(result.begin(), result.end(),
         [](pair<int, float> &a, pair<int, float> &b) {
             return a.second > b.second;
         });

    std::vector<Point> recommend;
    for (int i = 0; i < 5; ++i) {
        int tx = result[i].first / BOARD_SIZE;
        int ty = result[i].first % BOARD_SIZE;
        if (s.board[tx][ty] == EMPTY_GRID) {
            recommend.push_back(Point(tx, ty));
        }
    }

    return recommend;
}