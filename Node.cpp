#include "Node.h"
#include <iostream>
#include "math.h"
using namespace std;

Node::Node(const State& s, const Point& p, Node* parent, Node* sibling)
    : win(0.0),
      visit(0.0),
      score(0.0),
      move_from(p),
      parent(parent),
      sibling(sibling),
      children(nullptr),
      expended(false),
      move(s.all_move()),
      color(s.get_color()) {
    if (move.empty()) expended = true;
}

Node::Node(const State& s, std::vector<Point> move)
    : win(0.0),
      visit(0.0),
      score(0.0),
      move_from(Point(-1, -1)),
      parent(nullptr),
      sibling(nullptr),
      children(nullptr),
      expended(false),
      move(move),
      color(s.get_color()) {
    if (move.empty()) expended = true;
}

Node::~Node() {}

Point Node::get_move() {
    Point p = move.back();
    move.pop_back();
    if (move.empty()) expended = true;
    return p;
}

Node* Node::select_child(const double c) {
    double inv = 0.0;
    double max = 0.0;
    Node* p = this->children;
    Node* best_child = nullptr;
    while (p != nullptr) {
        inv = 1.0 / p->visit;
        p->score = p->win * inv + sqrt(c * log(this->visit) * inv);
        if (p->score >= max) {
            best_child = p;
            max = p->score;
        }
        p = p->sibling;
    }
    return best_child;
}

void Node::info() const {
    Node* ptr = children;
    while (ptr != nullptr) {
        cout << "Child from " << ptr->from();
        cout << "win: " << ptr->win << endl;
        cout << "visit: " << ptr->visit << endl;
        cout << "winning rate: " << ptr->win / ptr->visit << endl;
        cout << "score: " << ptr->score << endl;
        cout << endl;
        ptr = ptr->sibling;
    }
}