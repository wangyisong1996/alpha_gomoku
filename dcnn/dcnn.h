#ifndef __DCNN_H__
#define __DCNN_H__

#include <vector>
#include <memory>
#include <string>
#include "Point.h"
#include "Constants.h"
#include "State.h"

#define CPU_ONLY 1
#include <caffe/caffe.hpp>

class State;

class CNN {
public:
	CNN(const int color);
	~CNN();

	std::vector<Point> get_moves(const State& s) const;

private:
	int color;
	int connect_num(const int board[][BOARD_SIZE], const int x, const int y) const;
	static const int channel;
	static std::shared_ptr<caffe::Net<float> > net;
};

#endif // __DCNN_H__
