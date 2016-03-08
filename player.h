#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.h"
#include "board.h"
using namespace std;

struct node
{
	Board* board;
	Move* move;
	int score;
	std::vector<node*> children;
	node* parent;
	node(Board* b, Move* m)
	{
		board = b;
		move = m;
		parent = NULL;
	};

	void add_child(node* child)
	{
		children.push_back(child);
		child->parent = this;
	}
};

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board *board;
    Side side;
    int value(int move);
    void ValidMove(node* tree, Side side, int depth);
   
};

#endif
