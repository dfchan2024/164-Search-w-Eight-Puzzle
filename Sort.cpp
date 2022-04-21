#include <iostream>
#include <queue>
#include "Node.h"
#include "Hash.h"
#include "Sort.h"

// Default constructor
Sort::Sort() {
	initial = new Node();
	final = "";
	priority = new PQueue();
	type = false;
	numExp = 0;
	cutoff = new Node();
	next = new Node();
	result = new Node();
	fail = new Node();
	reach = new Node();
}

// Default constructor with initial and goal state and tyoe parameters
Sort::Sort(std::string state, std::string fin, bool type) {
	initial = new Node(state);
	final = fin;
	priority = new PQueue(type);
	this->type = type;
	numExp = 0;
	cutoff = new Node("0");
	next = new Node();
	result = new Node("0");
	fail = new Node();
	reach = new Node(state);
}

// Destructor
Sort::~Sort() {
	delete initial;
	delete cutoff;
	delete next;
	delete result;
	delete fail;
}

// Function to expand parent node to children nodes
Node* Sort::expand(Node* n, int i) {
	char last = ' ';
	if (!(*n).moves->empty())
		last = (*n).moves->back();

	if (i == 0 && last != 'D') {		// Returns child one by one; Checks so there are no backtracks (i.e., Down after an Up)
		if (n->valid('U'))				// If invalid move, return invalid state
			return (*n).moveAct('U');
	}
	else if (i == 1 && last != 'U') {
		if (n->valid('D'))
			return (*n).moveAct('D');
	}
	else if (i == 2 && last != 'R') {
		if (n->valid('L'))
			return (*n).moveAct('L');
	}
	else if (i == 3 && last != 'L') {
		if (n->valid('R'))
			return (*n).moveAct('R');
	}
	return fail;
}

// Function to perform Breadth-First-Search
Node* Sort::BFS() {
	Hash* reached;
	if (initial->state->size() == 16)
		reached = new Hash(200000);
	else
		reached = new Hash(5000);
	Node* n = initial;				// node <- NODE(problem.initial)
	if ((*n->state) == final)		// if problem.IS_GOAL(node.STATE) then return node
		return n;
	frontier.push(n);				// frontier <- FIFO queue, with node as an element
	reached->insert(*(n->state), (n->getHeur(true)));	// reached <- {problem.INITIAL}

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
				inReach = reached->search(*(next->state));
				if (!inReach) {									// if s is not in reached then
					reached->insert(s, (next->getHeur(true)));	// add s to reached
					frontier.push(next);						// add child to frontier
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
	return fail;
}

// Function to perform A* Search
Node* Sort::AstarSearch() {
	Hash* reached;
	if (initial->state->size() == 16)
		reached = new Hash(200000);
	else
		reached = new Hash(5000);
	Node* n = initial;				// node <- NODE(STATE=problem.INITIAL)
	priority->push(n);				// frontier <- a priority queue ordered by f, with node as an element
	reached->insert(*(n->state), (n->getHeur(type)));	// reached <- {problem.INITIAL}

	std::string s = "";
	bool inReach;

	while (!priority->isEmpty()) {			// while not IS_EMPTY(frontier) do
		n = priority->pop();				// node <- POP(frontier)
		if ((*n->state) == final)			// if problem.IS_GOAL(node.STATE) then return node
			return n;
		numExp++;
		for (int i = 0; i < 4; i++) {		// for each child in EXPAND(problem, node)
			next = expand(n, i);
			if (*(next->state) != std::string("")) {
				s = *(next->state);			// s <- child.STATE
				inReach = reached->search(*(next->state));
				if (inReach) {
					reach->setState(s);
					reach->setHeur(reached->getCost(s), type);
				}
				// if s is not in reached or child.PATH_COST < reached[s].PATH_COST then
				if (!inReach) {
					reached->insert(s, next->getHeur(type));	// reached[s] <- child
					priority->push(next);						// add child to frontier
				}
				else if (next->getHeur(type) < reach->getHeur(type)) {
					reached->setCost(s, next->getHeur(type));	// reached[s] <- child
					priority->push(next);						// add child to frontier
				}
			}
		}
	}
	return fail;		// return failure
}

// Function to perform Iterative Deeping A* Search
Node* Sort::IDASearch() {
	Hash* reached;
	if (initial->state->size() == 16)
		reached = new Hash(200000);
	else
		reached = new Hash(5000);
	Node* n = initial;									// node <- NODE(STATE=problem.INITIAL)
	priority->push(n);									// frontier <- a priority queue ordered by f, with node as an element
	reached->insert(*(n->state), (n->getHeur(type)));
	int fmax = n->getHeur(type);						// fmax <- h(initial state)

	for (int i = 0; i < INT_MAX; i++) {					// for i <- 0 to infinity do
		result = LimitedFSearch(fmax, reached);			// result <- LIMITED_F_SEARCH(problem, fmax)
		if ((*result->state) == final) {				// if result is a solution then return result
			std::cout << "Backtracked to heuristic cost: " << fmax << std::endl << std::endl;
			return result;
		}
		fmax = result->getHeur(type);					// else fmax <- result
	}
	return fail;
}

// Helper function for IDA; Combined DFS and A* Search
Node* Sort::LimitedFSearch(int fmax, Hash* reached) {
	std::string s = "";
	bool inReach;
	Node* n;

	while (!priority->isEmpty()) {
		n = priority->pop();
		if (*n->state == final)					// if search finds a solution then return the solution
			return n;
		else {
			numExp++;
			for (int i = 0; i < 4; i++) {		// for each child in EXPAND(problem, node)
				next = expand(n, i);
				if (*(next->state) != std::string("")) {
					s = *(next->state);			// s <- child.STATE
					inReach = (reached->search(*(next->state)));
					if (inReach) {
						reach->setState(s);
						reach->setHeur(reached->getCost(s), type);
					}
					// if s is not in reached or child.PATH_COST < reached[s].PATH_COST then
					if (!inReach) {
						(reached->insert(s, next->getHeur(type)));	// reached[s] <- child
						priority->push(next);						// add child to frontier
					}
					else if (next->getHeur(type) < reach->getHeur(type)) {
						(reached->setCost(s, next->getHeur(type)));	// reached[s] <- child
						priority->push(next);						// add child to frontier
					}
				}
			}
			if (n->getHeur(type) > fmax)		// else return min{f(m) | the search backtracked at node m}
				return n;
		}
	}
	return fail;
}
