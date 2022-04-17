#include <iostream>
#include <queue>
#include "Node.h"
#include "Sort.h"

// Default constructor
Sort::Sort() {
	initial = new Node();
	final = "";
	reached = new std::string[0];
	priority = new PQueue();
	numExp = 0;
	numNode = 0;
	cutoff = new Node();
	next = new Node();
	result = new Node();
	fail = new Node();
}

// Default constructor with initial and goal state parameters
Sort::Sort(std::string state, std::string fin) {
	initial = new Node(state);
	final = fin;
	reached = new std::string[1000000];
	priority = new PQueue();
	numExp = 0;
	numNode = 0;
	cutoff = new Node("0");
	next = new Node();
	result = new Node("0");
	fail = new Node();
}

// Destructor
Sort::~Sort() {
	delete[] reached;
	delete initial;
}

// Function to expand parent node to children nodes
Node* Sort::expand(Node* n, int i) {
	char last = ' ';
	if (!(*n).moves->empty())
		last = (*n).moves->back();

	if (i == 0 && last != 'D')			// Returns child one by one; Checks so there are no backtracks (i.e., Down after an Up)
		return (*n).moveAct('U');
	else if (i == 1 && last != 'U')
		return (*n).moveAct('D');
	else if (i == 2 && last != 'R')
		return (*n).moveAct('L');
	else if (i == 3 && last != 'L')
		return (*n).moveAct('R');
	else {
		Node* emp = new Node();
		return emp;
	}
}

// Function to perform Breadth-First-Search
Node* Sort::BFS() {
	Node* n = initial;				// node <- NODE(problem.initial)
	if ((*n->state) == final)		// if problem.IS_GOAL(node.STATE) then return node
		return n;
	frontier.push(n);				// frontier <- FIFO queue, with node as an element
	numNode++;
	reached[0] = *(n->state);		// reached <- {problem.INITIAL}

	std::string s = "";
	bool inReach = false;

	while (!frontier.empty()) {		// while not IS_EMPTY(frontier) do
		n = frontier.front();		// node <- POP(frontier)
		frontier.pop();
		numExp++;
		for (int i = 0; i < 4; i++) {		// for each child in EXPAND(problem, node) do
			next = expand(n, i);
			if (*(next->state) != std::string("")) {
				s = *(next->state);			// s <- child.STATE
				if (s == final)				// if problem.IS_GOAL(s) then return child
					return next;
				inReach = false;
				for (unsigned int j = 0; j < numNode; j++) {
					if (s == reached[j]) {
						inReach = true;
						j = numNode;
					}
				}
				if (!inReach) {				// if s is not in reached then
					reached[numNode] = s;	// add s to reached
					numNode++;
					frontier.push(next);	// add child to frontier
				}
			}
		}
	}
	return fail;		// return failure
}

// Function to perform Depth-First-Search
Node* Sort::DFS(int limit) {
	Node* n = initial;
	return RecursiveDFS(n, limit);		// RECURSIVE_DLS(MAKE_NODE(INITIAL_STATE[problem]), problem, limit)
}

// Helper function for DFS
Node* Sort::RecursiveDFS(Node* n, int limit) {
	bool cut = false;						// cutoff_occurred? <- false
	if ((*n->state) == final)				// if GOAL_TEST(problem, STATE[node]) then return node
		return n;
	else if (n->moves->size() == limit)		// else if DEPTH[node] = limit then return cutoff
		return cutoff;
	else {
		numExp++;
		for (int i = 0; i < 4; i++) {		// else for each successor in EXPAND(node, problem) do
			next = expand(n, i);
			if (*(next->state) != std::string("")) {
				result = RecursiveDFS(next, limit);				// result <- RECURSIVE_DLS(successor, problem, limit)
				if (*(result->state) == std::string("0"))		// if result = cutoff then cutoff_occurred? <- true
					cut = true;
				else if (*(result->state) != std::string(""))	// else if result != failure the return result
					return result;
			}
		}
	}
	if (cut)			// if cutoff_occurred? then return cutoff else return failure
		return cutoff;
	else {
		return fail;
	}
}

// Function to perform Iterative Deeping Search
Node* Sort::IterativeDeepingSearch() {
	for (int i = 0; i < INT_MAX; i++) {				// for depth <- 0 to infinity do
		result = DFS(i);							// result <- DEPTH_LIMITED_SEARCH(problem, depth)
		if (*(result->state) != std::string("0"))	// if result != cutoff then return result
			return result;
	}
	return result;
}

// Function to perform A* Search
Node* Sort::AstarSearch() {
	Node* n = initial;				// node <- NODE(STATE=problem.INITIAL)
	priority->push(n);				// frontier <- a priority queue ordered by f, with node as an element
	numNode++;
	reached[0] = (*n->state);		// reached <- {problem.INITIAL}

	std::string s = "";
	bool inReach;
	int reachVal;
	Node* reach = new Node(*n->state);

	while (!priority->isEmpty()) {			// while not IS_EMPTY(frontier) do
		n = priority->pop();				// node <- POP(frontier)
		if ((*n->state) == final)			// if problem.IS_GOAL(node.STATE) then return node
			return n;
		numExp++;
		for (int i = 0; i < 4; i++) {		// for each child in EXPAND(problem, node)
			next = expand(n, i);
			if (*(next->state) != std::string("")) {
				s = *(next->state);			// s <- child.STATE
				inReach = false;
				reachVal = numNode;
				for (unsigned int j = 0; j < numNode; j++) {
					if (s == reached[j]) {
						inReach = true;
						reachVal = j;
						reach->setState(reached[j]);
						j = numNode;
					}
				}
				// if s is not in reached or child.PATH_COST < reached[s].PATH_COST then
				if (!inReach || next->getHeur() < reach->getHeur()) {
					reached[reachVal] = (*next->state);		// reached[s] <- child
					priority->push(next);					// add child to frontier
					numNode++;
				}
			}
		}
	}
	return fail;		// return failure
}

// Function to perform Iterative Deeping A* Search
Node* Sort::IDASearch() {
	Node* n = initial;
	numNode++;
	reached[0] = (*n->state);
	int fmax = n->getHeur();				// fmax <- h(initial state)
	Node* result = new Node();
	for (int i = 0; i < INT_MAX; i++) {		// for i <- 0 to infinity do
		result = LimitedFSearch(n, fmax);	// result <- LIMITED_F_SEARCH(problem, fmax)
		if ((*result->state) == final) {	// if result is a solution then return result
			std::cout << "Backtracked to heuristic cost: " << fmax << std::endl << std::endl;
			return result;
		}
		fmax = result->getHeur();			// else fmax <- result
		n = result;
	}
	return result;
}

// Helper function for IDA; Combined DFS and A* Search
Node* Sort::LimitedFSearch(Node* n, int fmax) {
	if ((*n->state) == final)			// if GOAL_TEST(problem, STATE[node]) then return node
		return n;
	else if (n->getHeur() > fmax)		// return min {f(m) | search backtracked at node m}
		return n;
	else {
		std::string s = "";
		bool inReach;
		int reachVal;
		Node* reach = new Node(*n->state);

		numExp++;
		for (int i = 0; i < 4; i++) {					// for each child in EXPAND(problem, node)
			next = expand(n, i);
			if (*(next->state) != std::string("")) {
				s = *(next->state);						// s <- child.STATE
				inReach = false;
				reachVal = numNode;
				for (unsigned int j = 0; j < numNode; j++) {
					if (s == reached[j]) {
						inReach = true;
						reachVal = j;
						reach->setState(reached[j]);
						j = numNode;
					}
				}
				// if s is not in reached or child.PATH_COST < reached[s].PATH_COST then
				if (!inReach || next->getHeur() < reach->getHeur()) {
					reached[reachVal] = (*next->state);			// reached[s] <- child
					priority->push(next);						// add child to frontier
					numNode++;
				}
			}
		}
		n = priority->pop();									// node <- POP(frontier)
		return LimitedFSearch(n, fmax);							// result <- LIMITED_F_SEARCH(problem, fmax)
	}
}