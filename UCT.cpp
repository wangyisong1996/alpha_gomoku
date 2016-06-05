#include "UCT.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include "math.h"
#include "node.h"
using namespace std;

Xorshift Tree::random(32563345, 12343, 5646546, 123446543);

Tree::Tree(const State& s, const int max_itr, const double max_time)
    : root_state(s), max_itr(max_itr), max_time(max_time) {
    root = new Node(s, Point(-1, -1));
}

Tree::~Tree() { destory(root); }

void Tree::destory(Node* p) {
    if (!p) return;
    destory(p->children);
    destory(p->sibling);
    delete p;
}

Point Tree::UCT() {
    Point p(0, 0);
    clock_t max_clock = clock() + clock_t(max_time * CLOCKS_PER_SEC);
    for (int itr = 0; itr < max_itr; ++itr) {
        State state = root_state;
        Node* node = root;
        // Tree Policy
        while (!node->terminal()) {
            if (node->extensive()) {
                // expansion
                p = node->get_move();
                state.move(p);
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
        }

        // back propagation
        double reward = state.reward(node->color);
        while (node != nullptr) {
            node->update(reward);
            node = node->parent;
            reward = 1.0 - reward;
        }

        if (!(itr & 255) && clock() > max_clock) {
            cout << "Iteration:" << itr << endl;
            break;
        }
    }

    return root->select_child(0.0)->from();
}