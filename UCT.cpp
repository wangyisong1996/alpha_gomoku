#include "UCT.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cassert>
#include "math.h"
#include "node.h"
using namespace std;

Xorshift Tree::random(32563345, 12343, 5646546, 123446543);


Tree::Tree(const State& s) : root_state(s) {
    max_itr = 100000;
    max_time = 5.0;
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
                // cout << state << endl << endl;
                // getchar();
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
            // cout << state << endl << endl;
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
   
    // Node* ptr = root->children;
    // while (ptr != nullptr) {
    //     cout << "Child from " << ptr->from();
    //     cout << "win: " << ptr->win << endl;
    //     cout << "visit: " << ptr->visit << endl;
    //     cout << "winning rate: " << ptr->win / ptr->visit << endl;
    //     cout << "score: " << ptr->score << endl;
    //     cout << endl;
    //     ptr = ptr->sibling;
    // }
    // getchar();
    return root->select_child(0.0)->from();
}