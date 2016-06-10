#ifndef __UCT_H__
#define __UCT_H__

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "Constants.h"
#include "Node.h"
#include "RNG.h"
#include "State.h"

class Node;

class Tree {
public:
    Tree(const State& s, const int round, const int max_itr = 100000,
         const double max_time = 1);
    ~Tree();

    Point UCT();

private:
    void destory(Node* p);

    Point random_move() {
        return Point(random.rand() % BOARD_SIZE, random.rand() % BOARD_SIZE);
        ;
    }

    static Xorshift random;
    Node* root;
    State root_state;

    int max_itr;
    double max_time;
};

#endif  // __UCT_H__
