#include <iostream>
#include <queue>
#include <chrono>
#include "Sort.h"
#include "Node.h"

// Function to print menu
void printOpt() {
	std::cout << "1. Breadth-First Search" << std::endl;
	std::cout << "2. Depth-First Search" << std::endl;
	std::cout << "3. Iterative-Deeping Depth-First Search" << std::endl;
	std::cout << "4. A* Search w/ Out-Of-Place Heuristic" << std::endl;
	std::cout << "5. A* Search w/ Manhattan Distance Heuristic" << std::endl;
	std::cout << "6. Iterative-Deeping A* Search w/ Out-Of-Place Heuristic" << std::endl;
	std::cout << "7. Iterative-Deeping A* Search w/ Manhattan Distance Heuristic" << std::endl;
	std::cout << "8. Manual Puzzle Sequence" << std::endl;
}

int main() {
	// Beginning menu screen
	std::cout << "CSci 164 - Search w/Eight Puzzle Project" << std::endl << std::endl;
	std::cout << "Author: Derek Chan" << std::endl;
	std::cout << "Professor: Dr. Ruby" << std::endl;
	std::cout << "Date: 04/17/21" << std::endl << std::endl;

	std::cout << "Enter puzzle size (8 or 15):" << std::endl;
	std::string input;
	std::cin >> input;		// Get user input
	system("cls");			// clear the commandline (to refresh the screen)

	// Checks whether user input for puzzle size was 8 or 15; Loops for user input if not.
	while (input != std::string("8") && input != std::string("15")) {
		std::cout << "Invalid input. Please type valid puzzle size." << std::endl << std::endl;
		std::cout << "Enter puzzle size (8 or 15):" << std::endl;
		std::cin >> input;
		system("cls");
	}

	std::cout << "Which sorting algorithm would you like to use?" << std::endl;
	std::cout << "Enter the corresponding integer:" << std::endl;
	printOpt();
	int input2;
	std::cin >> input2;		// Get user input
	system("cls");

	// Checks whether user input for sorting algorithm was between 1 and 8; Loops for user input if not.
	while (input2 < 1 || input2 > 8) {
		std::cout << "Invalid input. Please type valid integer." << std::endl << std::endl;
		std::cout << "Enter the corresponding integer:" << std::endl;
		printOpt();
		std::cin >> input2;
		system("cls");
	}

	std::string input3;
	std::string input4;
	std::string input5;

	if (input == std::string("8")) {		// Case for 8-tile Puzzle
		std::cout << "Enter puzzle as string. (Ex: 160273485)" << std::endl;
		std::cin >> input3;			// Get user input

		// Checks whether user input for initial state was valid; Loops for user input if not.
		while (input3.size() != 9) {
			system("cls");
			std::cout << "Invalid input. Please type valid puzzle. (Ex: 160273485)" << std::endl;
			std::cin >> input3;
		}

		Node* startState = new Node(input3);
		startState->checkO();

		if (input2 == 8) {			// Case if manuel sequence is inputted
			std::cout << std::endl << "Enter move sequence as string. (Ex: UDLR)" << std::endl;
			std::cin >> input5;		// Get user input

			system("cls");
			startState->sequence(input5);
		}
		else {
			std::cout << std::endl;
			std::cout << "Enter goal state as string. (Ex: 123456780)" << std::endl;
			std::cin >> input4;		// Get user input

			// Checks whether user input for goal state was valid; Loops for user input if not.
			while (input4.size() != 9) {
				system("cls");
				std::cout << "Invalid input. Please type valid goal. (Ex: 123456780)" << std::endl;
				std::cin >> input4;
			}

			int lim = 0;
			if (input2 == 2) {			// Case if DFS is used
				std::cout << std::endl << "Please enter depth limit as integer." << std::endl;
				std::cin >> lim;		// Get user input
			}

			Node* goalState = new Node(input4);
			goalState->checkO();
			system("cls");

			startState->display();		// Display tile puzzle

			// Create puzzle object from Sort class
			Sort* Puzzle;
			if (input2 == 5 || input2 == 7)
				Puzzle = new Sort(input3, input4, false);		// For A* with Manhanttan Distance
			else
				Puzzle = new Sort(input3, input4, true);		// For A* with Out-Of-Place
			Node* fin = new Node();

			// Start of stopwatch
			auto start = std::chrono::high_resolution_clock::now();

			// Perform one of the seven sorting algorithms
			if (input2 == 1) {
				fin = Puzzle->BFS();
			}
			else if (input2 == 2) {
				std::cout << "Depth Limit: " << lim << std::endl << std::endl;
				fin = Puzzle->DFS(lim);
			}
			else if (input2 == 3) {
				fin = Puzzle->IterativeDeepingSearch();
			}
			else if (input2 == 4) {
				std::cout << "Heuristics Cost: " << startState->getHeur(true) << std::endl << std::endl;
				fin = Puzzle->AstarSearch();
			}
			else if (input2 == 5) {
				std::cout << "Heuristics Cost: " << startState->getHeur(false) << std::endl << std::endl;
				fin = Puzzle->AstarSearch();
			}
			else if (input2 == 6) {
				std::cout << "Heuristics Cost: " << startState->getHeur(true) << std::endl << std::endl;
				fin = Puzzle->IDASearch();
			}
			else if (input2 == 7) {
				std::cout << "Heuristics Cost: " << startState->getHeur(false) << std::endl << std::endl;
				fin = Puzzle->IDASearch();
			}

			// End of stopwatch
			auto end = std::chrono::high_resolution_clock::now();
			double time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			if ((*fin->state) == std::string(""))			// Case if algorithm could not reach goal state
				std::cout << "Final state could not be found. Please relaunch program and try again." << std::endl;
			else if ((*fin->state) == std::string("0"))		// Case if depth limit for DFS is reached
				std::cout << "Depth limit reached. Final state could not be found. Please relaunch program and try again." << std::endl;
			else {
				int ex = Puzzle->getNumExp();
				std::string m = (*fin->moves);

				// Print final details of sorting algorithm
				std::cout << "Final state:" << std::endl;
				fin->display();
				std::cout << "Number of expanded node: " << ex << std::endl;
				std::cout << "Solution sequence of moves: " << m << std::endl;
				if (input2 == 3)
					std::cout << "Depth Reached: " << m.size() << std::endl;
				std::cout << "Time take: " << time_taken << " milliseconds" << std::endl;
			}
		}
	}
	else if (input == std::string("15")) {			// Case for 15-tile Puzzle
		std::cout << "Enter puzzle as string. (Ex: 16235A749C08DEBF)" << std::endl;
		std::cin >> input3;			// Get user input

		// Checks whether user input for initial state was valid; Loops for user input if not.
		while (input3.size() != 16) {
			system("cls");
			std::cout << "Invalid input. Please type valid puzzle. (Ex: 16235A749C08DEBF)" << std::endl;
			std::cin >> input3;
		}

		Node* startState = new Node(input3);
		startState->checkO();

		if (input2 == 8) {			// Case if manuel sequence is inputted
			std::cout << std::endl << "Enter move sequence as string. (Ex: UDLR)" << std::endl;
			std::cin >> input5;		// Get user input
			system("cls");
			startState->sequence(input5);
		}
		else {
			std::cout << std::endl;
			std::cout << "Enter goal state as string. (Ex: 123456789ABCDEF0)" << std::endl;
			std::cin >> input4;		// Get user input

			// Checks whether user input for goal state was valid; Loops for user input if not.
			while (input4.size() != 16) {
				system("cls");
				std::cout << "Invalid input. Please type valid goal. (Ex: 123456789ABCDEF0)" << std::endl;
				std::cin >> input4;
			}

			int lim = 0;
			if (input2 == 2) {		// Case if DFS is used
				std::cout << std::endl << "Please enter depth limit as integer." << std::endl;
				std::cin >> lim;	// Get user input
			}

			Node* goalState = new Node(input4);
			goalState->checkO();
			system("cls");

			startState->display();		// Display tile puzzle

			// Create puzzle object from Sort class
			Sort* Puzzle;
			if (input2 == 5 || input2 == 7)
				Puzzle = new Sort(input3, input4, false);		// For A* with Manhanttan Distance
			else
				Puzzle = new Sort(input3, input4, true);		// For A* with Out-Of-Place
			Node* fin = new Node();

			// Start of stopwatch
			auto start = std::chrono::high_resolution_clock::now();

			// Perform one of the seven sorting algorithms
			if (input2 == 1) {
				fin = Puzzle->BFS();
			}
			else if (input2 == 2) {
				std::cout << "Depth Limit: " << lim << std::endl << std::endl;
				fin = Puzzle->DFS(lim);
			}
			else if (input2 == 3) {
				fin = Puzzle->IterativeDeepingSearch();
			}
			else if (input2 == 4) {
				std::cout << "Heuristics Cost: " << startState->getHeur(true) << std::endl << std::endl;
				fin = Puzzle->AstarSearch();
			}
			else if (input2 == 5) {
				std::cout << "Heuristics Cost: " << startState->getHeur(false) << std::endl << std::endl;
				fin = Puzzle->AstarSearch();
			}
			else if (input2 == 6) {
				std::cout << "Heuristics Cost: " << startState->getHeur(true) << std::endl << std::endl;
				fin = Puzzle->IDASearch();
			}
			else if (input2 == 7) {
				std::cout << "Heuristics Cost: " << startState->getHeur(false) << std::endl << std::endl;
				fin = Puzzle->IDASearch();
			}

			// End of stopwatch
			auto end = std::chrono::high_resolution_clock::now();
			double time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

			if ((*fin->state) == std::string(""))			// Case if algorithm could not reach goal state
				std::cout << "Final state could not be found. Please relaunch program and try again." << std::endl;
			else if ((*fin->state) == std::string("0"))		// Case if depth limit for DFS is reached
				std::cout << "Depth limit reached. Final state could not be found. Please relaunch program and try again." << std::endl;
			else {
				int ex = Puzzle->getNumExp();
				std::string m = (*fin->moves);

				// Print final details of sorting algorithm
				std::cout << "Final state:" << std::endl;
				fin->display();
				std::cout << "Number of expanded node: " << ex << std::endl;
				std::cout << "Solution sequence of moves: " << m << std::endl;
				if (input2 == 3)
					std::cout << "Depth Reached: " << m.size() << std::endl;
				std::cout << "Time take: " << time_taken << " milliseconds" << std::endl;
			}
		}
	}
	return 0;
}