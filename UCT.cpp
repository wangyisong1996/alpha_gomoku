#include "UCT.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include "math.h"
using namespace std;

Xorshift tree::rand(233, 12343, 5646546, 123446543);

tree_node::tree_node(const State& s, const Point& p, tree_node* parent)
    : win(0.0),
      visit(0.0),
      score(0.0),
      move_from(p),
      parent(parent),
      expended(false),
      has_child(false),
      move(s.all_move()),
      color(s.get_color()) {
        if (move.empty())
            expended = true;
      }

tree_node::~tree_node() {
    for (auto&& i : children) {
        delete i;
    }
}

tree_node* tree_node::add_child(const State& s, const Point& p) {
    tree_node* child = new tree_node(s, p, this);
    children.push_back(child);
    has_child = true;
    return child;
}

Point tree_node::get_move() {
    Point p = move.back();
    move.pop_back();
    if (move.empty()) expended = true;
    return p;
}

tree_node* tree_node::select_child(const double c) {
    double inv = 0.0;
    for (auto&& i : children) {
        inv = 1.0 / i->visit;
        i->score = i->win * inv + sqrt(c * log(this->visit) * inv);
    }
    return *max_element(
        children.begin(), children.end(),
        [](tree_node* a, tree_node* b) { return a->score < b->score; });
}

tree::tree(const State& s) : root_state(s), root(s, Point(-1, -1)) {
    max_itr = 400000;
    max_time = 10.0;
}

tree::~tree() {}

Point tree::uct() {
    clock_t max_clock = clock() + clock_t(max_time * CLOCKS_PER_SEC);
    Point p(0, 0);
    // cout << root_state << endl;
    for (int itr = 0; itr < max_itr; ++itr) {
        State state = root_state;
        tree_node* node = &root;

        // Tree Policy
        while (!node->terminal()) {
            if (node->extensive()) {
                // expansion
                p = node->get_move();
                node = node->add_child(state, p);
                break;
            } else {
                // selection
                node = node->select_child();
                state.move(node->from());
            }
        }

        while (state.can_move()) {
            do {
                p = random_move();
            } while (!state.can_move(p));
            state.move(p);
            // cout << state << endl;
        }

        // back propagation
        double reward = state.reward(node->color);
        while (node != nullptr) {
            node->update(reward);
            node = node->parent;
            reward = 1.0 - reward;
        }

        if (!(itr & 255) && clock() > max_clock) {
            break;
        }
    }
    // for (auto&& i : root.children) {
    //     if (1) {
    //         State s = root_state;
    //         s.move(i->from());
    //         if (s.get_winner() == root.color) {
    //             cout << s << endl;
    //             if (i->terminal())
    //                 cout << "terminal!" << endl;
    //             p = i->from();
    //             break;
    //         }
    //     }
    // }
    return root.select_child(0.0)->from();
}