#include <iostream>
#include <math.h>
#include "Node.h"

// Default constructor
Node::Node() {
	O_tile = -1;
	size = 0;
	outOfPlace = manhanDis = heuristic = 0;
	state = new std::string("");
	moves = new std::string("");
}

// Default constructor with state parameter
Node::Node(std::string s) {
	state = new std::string(s);
	size = state->size();
	O_tile = -1;
	for (int i = 0; i < size; i++) {		// Loop through state to find blank
		if (state->at(i) == '0')
			O_tile = i;
	}
	checkO();
	moves = new std::string("");
	CalHeur();								// Call function to calculate heuristic costs
}

// Default constructor with state and moves parameter
Node::Node(std::string s, std::string m) {
	state = new std::string(s);
	size = state->size();
	O_tile = -1;
	for (int i = 0; i < size; i++) {		// Loop through state to find blank
		if (state->at(i) == '0')
			O_tile = i;
	}
	checkO();
	moves = new std::string(m);
	CalHeur();								// Call function to calculate heuristic costs
}

// Destructor
Node::~Node() {
	delete state;
	delete moves;
}

// Function to calculate all heuristic costs of tile puzzle
void Node::CalHeur() {
	outOfPlace = manhanDis = heuristic = 0;
	int val = 0;
	int sq = sqrt(state->size());
	int depth = moves->size();

	for (int i = 0; i < state->size(); i++) {		// Calculate Out-Of-Place
		val = (state->at(i) - 48);
		if (val != i+1 && val != 0)
			outOfPlace++;
	}
	for (int i = 0; i < state->size(); i++) {		// Calculate Manhanttan Distance
		val = (state->at(i) - 48);
		manhanDis += (abs(((val - 1) % sq) - (i % sq)) + abs(((val - 1) / sq) - (i / sq)));
	}
	heuristic = outOfPlace + manhanDis + depth;		// Add cost so far
}

// Function to display tile puzzle as 3x3 or 4x4
void Node::display() {
	if (size == 9 || size == 16) {
		int sq = sqrt(size);
		for (int i = 0; i < size; i++) {
			std::cout << state->at(i);
			if (i % sq == (sq - 1))
				std::cout << std::endl;
		}
	}
	else {		// Case where puzzle isn't 8 or 15 tiles
		for (int i = 0; i < size; i++)
			std::cout << state->at(i);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Function to change current state into new string
void Node::setState(std::string s) {
	if (s.size() != size) {		// Case where new string doesn't match state's size
		system("cls");
		std::string input;
		std::cout << "Invalid input. Please type valid puzzle. (Ex: 160273485)" << std::endl;
		
		std::cin >> input;
		setState(input);
	}
	else {
		(*state) = s;
		O_tile = -1;
		for (int i = 0; i < size; i++) {		// Loop through state to find blank
			if (state->at(i) == '0')
				O_tile = i;
		}
		checkO();
	}
	CalHeur();									// Call function to calculate heuristic costs
}

// Function to check if blank is within puzzle
void Node::checkO() {
	if (O_tile == -1) {		// Case where blank is missing
		std::string input;
		std::cout << std::endl << "State has no blank. Please enter new state with 0 included:" << std::endl;

		std::cin >> input;
		setState(input);
	}
}

// Function to check if a move is valid on current state
bool Node::valid(char m) {
	int sq = sqrt(size);
	if (m == 'u' || m == 'U') {			// Case where move is Up
		if (O_tile < sq)
			return false;
		else
			return true;
	}
	else if (m == 'd' || m == 'D') {	// Case where move is Down
		if (O_tile >= (size - sq))
			return false;
		else
			return true;
	}
	else if (m == 'l' || m == 'L') {	// Case where move is Left
		if (O_tile % sq == 0)
			return false;
		else
			return true;
	}
	else if (m == 'r' || m == 'R') {	// Case where move is Right
		if (O_tile % sq == (sq - 1))
			return false;
		else
			return true;
	}
	else
		return false;					// Case where move is not valid character
}

// Helper function for moveAct; Returns a new state after a move from an initial state
std::string Node::swapStr(int x, int y) {
	std::string st = *state;
	std::swap(st.at(x), st.at(y));
	return st;
}

// Function to perform a move on puzzle tile
void Node::move(char m) {
	int temp;
	int sq = sqrt(size);
	switch (m) {										// Determine which tile will be moved
		case 'u': case 'U': temp = O_tile - sq;	break;
		case 'd': case 'D': temp = O_tile + sq;	break;
		case 'l': case 'L': temp = O_tile - 1;	break;
		case 'r': case 'R': temp = O_tile + 1;	break;
		default:  temp = -1;
	}
	std::swap(state->at(O_tile), state->at(temp));		// Swap tiles to simulate move
	moves->push_back(m);								// Adds move to path
	O_tile = temp;
	CalHeur();											// Call function to calculate heuristic costs
}

// Function to perform a move on puzzle tile and return as new Node
Node* Node::moveAct(char m) {
	Node* ret;
	if (!valid(m)) {		// Invalid move returns invalid state
		ret = new Node();
		return ret;
	}
	else {
		int temp;
		int sq = sqrt(size);
		switch (m) {										// Determine which tile will be moved
		case 'u': case 'U': temp = O_tile - sq;	break;
		case 'd': case 'D': temp = O_tile + sq;	break;
		case 'l': case 'L': temp = O_tile - 1;	break;
		case 'r': case 'R': temp = O_tile + 1;	break;
		default:  temp = -1;
		}
		std::string newState = swapStr(O_tile, temp);		// Calls helper function
		std::string newMove = *moves;
		newMove.push_back(m);
		ret = new Node(newState, newMove);
		return ret;											// Returns a new Node with state after a move from an initial state
	}
}

// Function to perform a sequence of moves for testing solutions
void Node::sequence(std::string s) {
	std::string temp = *state;
	bool invalid = false;

	display();
	std::cout << "Move sequence: " << s << std::endl << std::endl;

	for (unsigned int i = 0; i < s.size(); i++) {
		if (!valid(s[i])) {		// Case if an invalid move is attempted
			system("cls");
			std::cout << "Invalid move attempted." << std::endl;
			std::cout << "Attempted move: " << s[i] << std::endl;
			std::cout << "State:" << std::endl;
			display();
			i = 100;
			invalid = true;
		}
		else {					// Perform sequence move by move
			move(s[i]);
		}
	}

	if (!invalid) {
		std::cout << "New state:" << std::endl;
		display();				// Display new state
	}
	else {						// If an invalid move is attempted, ask user for new sequence
		std::string input;
		std::cout << "Please enter new sequence of moves:" << std::endl;
		std::cin >> input;
		system("cls");
		setState(temp);
		sequence(input);
	}
}