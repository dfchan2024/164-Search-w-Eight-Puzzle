#include <iostream>
#include "Node.h"
#include "PQueue.h"

using namespace std;

// Default constructor
PQueue::PQueue() {
	numElems = 0;
}

// Boolean function to check if PQ is empty
bool PQueue::isEmpty() {
	return (numElems == 0);
}

// Function to return the parent of an element
int PQueue::getParent(int child) {
	if (child % 2 == 0)
		return (child / 2) - 1;
	else
		return child / 2;
}

// Function to return the left child of an element
int PQueue::getLeftChild(int parent) {
	return (2 * parent + 1);
}

// Function to return the right child of an element
int PQueue::getRightChild(int parent) {
	return (2 * parent * 2);
}

// Function to push element onto PQ
void PQueue::push(Node* item) {
		int i = numElems;
		Pqueue.push_back(item);
		numElems++;
		minHeapify(0, numElems);
}

// Function to pop first element in PQ
Node* PQueue::pop() {
	if (numElems > 0) {
		Node* temp = Pqueue.front();
		swap(Pqueue.at(0), Pqueue.at(numElems - 1));	// Swap front element with last element of queue to pop out
		Pqueue.pop_back();
		numElems--;
		minHeapify(0, numElems);
		return temp;
	}
	else {
		Node* temp = new Node();
		return temp;
	}
}

// Function to converts binary tree to satisfy min heap property
void PQueue::minHeapify(int i, int size) {
	int left = 2 * i + 1;		// left child of a[i]
	int right = 2 * i + 2;		// right child of a[i]
	int smallest = i;			// original smallest element

	if ((left < size) && (Pqueue.at(left)->getHeur() < Pqueue.at(i)->getHeur()))
		smallest = left;
	if ((right < size) && (Pqueue.at(right)->getHeur() < Pqueue.at(smallest)->getHeur()))
		smallest = right;
	if (smallest != i) {		// smallest element was swapped
		swap(Pqueue.at(i), Pqueue.at(smallest));
		minHeapify(smallest, size);
	}
}

// function to build min heap from binary tree
void PQueue::buildMinHeap() {
	// Traverses complete tree backwards (right to left, bottom to top)
	for (int i = numElems / 2 - 1; i >= 0; i--)
		minHeapify(i, numElems);
}