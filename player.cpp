#include "player.h"
#include <vector>
#include <cstdlib>


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) 
{
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    this->side = side;
    board = new Board(); 
}

/*
 * Destructor for the player.
 */
Player::~Player() 
{
	delete board;
}

void Player::ValidMove(node* tree, Side side, int depth){
	/* 
     * The valid moves that each board has
     */ 

     if (depth == 0)
     {
     	return;
     }

     for (int i = 0; i < 8; i++)
     {
     	for (int j = 0; j < 8; j++)
     	{
     		Move*move = new Move(i, j);
     		if (board->checkMove(move, side))
     		{
     			Board*NewBoard = board->copy();
     			node*new_node = new node(NewBoard, move);
     			ValidMove(new_node, (side == BLACK ? WHITE : BLACK), depth-1);
     			tree->add_child(new_node);
     		} 
     	}
     }

}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 

     if (opponentsMove != NULL)
     {
     	board -> doMove (opponentsMove, (side == BLACK ? WHITE: BLACK));
     }

     if (!board->hasMoves(side))
     {
     	return NULL;
     }

     node* tree = new node(board, opponentsMove);
     ValidMove(tree, side, 4);

     std::vector<node *> plyone, plytwo, plythree, plyfour;
     plyone = tree->children;

     for (unsigned int i = 0; i < plyone.size(); i++)
     {
     	plytwo.insert(plytwo.end(), plyone[i]->children.begin(), plyone[i]->children.end());
     }

     for (unsigned int i = 0; i < plytwo.size(); i++)
     {
     	plythree.insert(plythree.end(), plytwo[i]->children.begin(), plytwo[i]->children.end());
     }

     for (unsigned int i = 0; i < plythree.size(); i++)
     {
     	plyfour.insert(plyfour.end(), plythree[i]->children.begin(), plythree[i]->children.end());
     }


     //Initialize as very large values

     int minone = 99999999;
	 int mintwo = 99999999;
	 int maxone = -99999999;
	 int maxtwo = -99999999;

	for (unsigned int i = 0; i < plyfour.size(); i++)
	{
		Move* x = plyfour[i]->move;
		plyfour[i]->score = value(8*(x->getX()) + x->getY());
	}

	for (unsigned int i = 0; i < plythree.size(); i++)
	{
		plythree[i]->score = mintwo;
		for (unsigned int child = 0; child < plythree[i]->children.size(); child ++)
		{
			if (plythree[i]->children[child]->score < plythree[i]->score)
			{
				plythree[i]->score = plythree[i]->children[child]->score;
			}
		}
	}

	for (unsigned int i = 0; i < plytwo.size(); i++)
	{
		plytwo[i]->score = maxone;
		for (unsigned int j = 0; j < plytwo[i]->children.size(); j++)
		{
			if (plytwo[i]->children[j]->score > plytwo[i]->score)
			{
				plytwo[i]->score = plytwo[i]->children[j]->score;
			}
		}
	}

	for (unsigned int i = 0; i < plyone.size(); i++)
	{
		plyone[i]->score = minone;
		for (unsigned int j = 0; j < plyone[i]->children.size(); j++)
		{
			if (plyone[i]->children[j]->score < plyone[i]->score)
			{
				plyone[i]->score = plyone[i]->children[j]->score;
			}
		}
	}

	Move* final_move;

	for (unsigned int i = 0; i < plyone.size(); i++)
	{
		if (plyone[i]->score > maxtwo)
		{
			maxtwo = plyone[i]->score;
			final_move = plyone[i]->move;
		}
	}

	std::cerr << final_move->getX() << " " << final_move->getY() << std::endl;
	board->doMove(final_move, side);
	return final_move;
}

int Player::value(int move) 

    /* 
     * Implementing heurestic -- asigning weights to different moves.
     * 
     */ 

{
	Board * board = this->board->copy();
	int value = board->count(side) - board->count((side == BLACK ? WHITE : BLACK));

	if (move == 0 || move == 7 || move == 56 || move == 63) 
	{
		value *= 3;
	}

	else if (move == 2 || move == 3 || move == 4 || move == 5 || move == 16 || move == 24 || 
			 move == 32 || move == 40 || move == 23 || move == 31 || move == 39 || 
			 move == 47 || move == 58 || move == 59 || move == 60 || move == 61)
	{
		value *= 2;
	}
	
	else if (move == 8 || move == 1 || move == 6 || move == 15 || 
		    move == 55 || move == 62 || move == 48 || move == 57)
	{
		value *= -2;
	}

	else if (move == 9 || move == 14 || move == 49 || move == 54)
	{
		value *= -3;
	}

	delete board;
	return value;
}