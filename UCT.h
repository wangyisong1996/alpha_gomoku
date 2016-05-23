#ifndef UCT_H
#define UCT_H

#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Constants.h"
#include "State.h"
#include "Rng.h"

class tree;

class tree_node {
public:
    tree_node(const State& s, const Point& p = Point(-1, -1), tree_node* parent = nullptr);
    ~tree_node();

    tree_node* add_child(const State& s, const Point& p);

    bool leaf() { return !has_child; }
    bool terminal() { return expended && !has_child; }
    bool extensive() { return !expended; }

    void update(double reward) {
    	++visit;
    	win += reward;
    }

    Point get_move();
    Point from() const { return move_from; }
    tree_node* select_child(double c = 7.0);

private:
    friend class tree;
    std::vector<tree_node*> children;
    std::vector<Point> move;
    tree_node* parent;
    Point move_from;

    double win, visit, score;
    int color;
    // fully expended
    bool expended;
    bool has_child;
    // tree_node *parent, *sibling, *children;
};

class tree {
public:
    tree(const State& s);
    ~tree();

    Point uct();

private:
    static Xorshift rand;
	// naive rand
    Point random_move() {
        return Point(rand.rand() % BOARD_SIZE, rand.rand() % BOARD_SIZE);
    }
    tree_node root;
    State root_state;

    int max_itr;
    double max_time;
};

#endif  // UCT_H
