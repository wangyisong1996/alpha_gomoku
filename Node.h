#ifndef __NODE_H__
#define __NODE_H__

#include <cstdlib>
#include <vector>
#include "Constants.h"
#include "RNG.h"
#include "State.h"
#include "UCT.h"

class Tree;

class Node {
public:
    Node(const State& s, const Point& p = Point(-1, -1), Node* parent = nullptr, Node* sibling = nullptr);
    // This constructor is used for creating root node
    Node(const State& s, std::vector<Point> move);

    ~Node();

    Node* add_child(const State& s, const Point& p) {
        return children = new Node(s, p, this, children);
    }

    bool leaf() { return !children; }
    bool terminal() { return expended && !children; }
    bool extensive() { return !expended; }

    void update(double reward) {
        ++visit;
        win += reward;
    }

    void info() const;

    Point get_move();
    Point from() const { return move_from; }
    Node* select_child(double c = 8.0);

private:
    friend class Tree;

    std::vector<Point> move;
    Node *parent, *sibling, *children;
    Point move_from;

    double win, visit, score;
    int color;
    bool expended;
};

#endif // __NODE_H__