#ifndef __UCT_H__
#define __UCT_H__

#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include "Constants.h"
#include "RNG.h"
#include "State.h"
#include "node.h"

class Node;

class Tree {
public:
    Tree(const State& s);
    ~Tree();

    Point UCT();

private:
    void destory(Node* p);
    
    Point random_move() {
        return Point(random.rand() % BOARD_SIZE, random.rand() % BOARD_SIZE);;
    }

    static Xorshift random;
    Node* root;
    State root_state;

    int max_itr;
    double max_time;
};

#endif  // UCT_H
