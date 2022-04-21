#include <iostream>
#include <vector>
#include "Node.h"
#ifndef PQUEUE_HEADER
#define PQUEUE_HEADER


// Priority Queue class to be used for A* Search 
class PQueue {
private:
	int numElems;							// Number of elements within PQ
	bool type;								// Bool to track which heuristic cost for A* is used
	std::vector<Node*> Pqueue;				// Queue made by vector of Node objects
	void minHeapify(int i, int size);		// function to converts binary tree to satisfy min heap property
public:
	PQueue();			// default constructor
	PQueue(bool t);
	~PQueue() {};		// destructor
	int getNumElems() { return numElems; }	// function to return number of elements within PQ
	void PQclear();							// function to clear priority queue
	bool isEmpty();							// boolean function to check if PQ is empty
	int getParent(int child);				// function to return the parent of an element
	int getLeftChild(int parent);			// function to return the left child of an element
	int getRightChild(int parent);			// function to return the right child of an element
	void push(Node* item);					// function to push element onto PQ
	Node* pop();							// function to pop first element in PQ
	void buildMinHeap();					// function to build min heap from binary tree
};

#endif