#include <iostream>
#ifndef NODE_HEADER
#define NODE_HEADER

// Node class to use for tile puzzle
class Node {
private:
	int size;			// number of tiles in puzzle (Including blank)
	int O_tile;			// position of blank tiles within string
	int outOfPlace;		// heuristic #1: Number of misplaced tiles
	int manhanDis;		// heuristic #2: Manhattan distance of all 8 tiles
	int heuristic;		// total heuristic cost
	void CalHeur();		// function to calculate all heuristic costs of tile puzzle

public:
	Node();					// default constructor
	Node(std::string s);
	Node(std::string s, std::string m);
	~Node();				// destructor
	std::string* state;		// tile puzzle stored as string
	std::string* moves;		// sequence of moves to get to current state from initial state

	void display();							// function to display tile puzzle as 3x3 or 4x4
	void setState(std::string s);			// function to change current state into new string
	void checkO();							// function to check if blank is within puzzle
	int getHeur() { return heuristic; }		// function to return total heuristic cost
	bool valid(char m);						// function to check if a move is valid on current state
	std::string swapStr(int x, int y);		// helper function for moveAct
	void move(char m);						// function to perform a move on puzzle tile
	Node* moveAct(char m);					// function to perform a move on puzzle tile and return as new Node
	void sequence(std::string s);			// function to perform a sequence of moves for testing solutions
};

#endif