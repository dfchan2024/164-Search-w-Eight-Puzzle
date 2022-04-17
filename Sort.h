#include <iostream>
#include <queue>
#include "Node.h"
#include "PQueue.h"
#ifndef SORT_HEADER
#define SORT_HEADER

// Sort class to use for sorting algorithms
class Sort {
private:
	Node* initial;					// initial puzzle state problem
	std::queue<Node*> frontier;		// frontier for BFS stored as queue
	PQueue* priority;				// priority queue for A* from PQueue class
	std::string* reached;			// reached stored array of states
	std::string final;				// final puzzle state stored as string
	unsigned int numExp;			// number of expanded Nodes
	unsigned int numNode;			// number of Nodes stored in reached

	Node* cutoff;					// cutOff Node used for DFS
	Node* next;						// next Node used for sorting algorithms
	Node* result;					// result Node used for sorting algorithms
	Node* fail;						// fail Node used for sorting algorithms

public:
	Sort();							// default constructor
	Sort(std::string state, std::string fin);
	~Sort();						// destructor

	int getNumExp() { return numExp; }				// function to return number of expanded Nodes
	Node* expand(Node* n, int i);					// function to expand parent node to children nodes
	Node* BFS();									// function to perform Breadth-First-Search
	Node* DFS(int limit);							// function to perform Depth-First-Search
	Node* RecursiveDFS(Node* n, int limit);			// helper function for DFS
	Node* IterativeDeepingSearch();					// function to perform Iterative Deeping Search
	Node* AstarSearch();							// function to perform A* Search
	Node* IDASearch();								// function to perform Iterative Deeping A* Search
	Node* LimitedFSearch(Node* n, int fmax);		// helper function for IDA
};


#endif