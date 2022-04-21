#include <iostream>
#include <math.h>
#include "Hash.h"

// Default constructor
Hash::Hash() {
	size = 0;
	count = 0;
	states = NULL;
}

// Default constructor with size parameter
Hash::Hash(int s) {
	size = s;
	count = 0;
	states = new Ht_item*[s];
	for (int i = 0; i < size; i++)
		states[i] = NULL;
}

// Destructor
Hash::~Hash() {
	for (int i = 0; i < size; i++) {
		delete states[i];
	}
	delete[] states;
}

// Function to apply hash to get key
int Hash::hashFunc(std::string state) {
	int ret = 0;
	int len = state.size();
	int fact;
	if (len == 9) {					// For 8-puzzle, apply binary multiplication to entire state string
		fact = len - 1;				// (Ex. 123456780 = (1 * 2^8) + (2 * 2^7) + ... (0 * 2^0))
		for (int i = 0; i < len; i++) {
			ret += (state[i] - '0') * pow(2, fact);
			fact--;
		}
		if ((ret / 502) > 1)
			ret -= 502;
		else
			ret %= 502;				// mod 502 to fill in locations 0 - 501 in Hash Table
	}
	else if (len == 16) {			// For 16-puzzle, apply binary multiplication to the middle 14 characters in state string
		fact = len - 3;				// (Ex. ABCDEF9876543210 = (11 * 2^13) + (12 * 2^12) + ... + (1 * 2^0))
		for (int i = 1; i < len - 1; i++) {
			if (state[i] > '9')
				ret += (state[i] - '7') * pow(2, fact);
			else
				ret += (state[i] - '0') * pow(2, fact);
			fact--;
		}
		if ((ret / 32752) > 1)
			ret -= 32752;
		else
			ret %= 32752;			// mid 32752 to fill in locations 0 - 32751 in Hash Table
	}

	return ret;
}

// Function to insert item into Hash Table
void Hash::insert(std::string state, int cost) {
	int k = hashFunc(state);			// Use hash function to get key
	Ht_item* item = new Ht_item;		// Create new item with key, state, and cost
	item->key = k;
	item->state = state;
	item->cost = cost;
	count++;

	if (states[k] != NULL) {			// Case if location at key is not empty, use linked list to find empty location and insert
		Ht_item** next = &states[k]->next;
		while ((*next) != NULL)
			next = &((*next)->next);
		(*next) = item;
		(*next)->next = NULL;
	}
	else {								// Case if location at key is empty, insert into Hash Table
		states[k] = item;
		states[k]->next = NULL;
	}
}

// Function to search item within Hash Table
bool Hash::search(std::string state) {
	int k = hashFunc(state);			// Use hash function to get key
	if (states[k] != NULL) {			// Case location at key is not empty, attempt to find state match
		if (states[k]->state == state)
			return true;				// State match at first element
		else {
			Ht_item** next = &states[k]->next;
			while ((*next) != NULL) {
				if ((*next)->state == state)
					return true;		// State match found in linked list
				next = &((*next)->next);
			}
		}
	}
	return false;						// Case where state not found in Hash Table
}

// Function to return item cost for item within Hash Table
int Hash::getCost(std::string state) {
	int k = hashFunc(state);				// Use hash function to get key

	if (states[k] != NULL) {				// Case location at key is not empty, attempt to find state match
		if (states[k]->state == state)
			return states[k]->cost;			// Item found at first element, return cost
		else {
			Ht_item** next = &states[k]->next;
			while ((*next) != NULL) {
				if ((*next)->state == state)
					return (*next)->cost;	// Item found in linked list, return cost
				next = &((*next)->next);
			}
		}
	}
	return 1000;							// Item not found, return large cost
}

void Hash::setCost(std::string state, int cost) {
	int k = hashFunc(state);				// Use hash function to get key

	if (states[k] != NULL) {				// Case location at key is not empty, attempt to find state match
		if (states[k]->state == state)
			states[k]->cost = cost;			// Item found at first element, update cost
		else {
			Ht_item** next = &states[k]->next;
			while ((*next) != NULL) {
				if ((*next)->state == state)
					(*next)->cost = cost;	// Item found in linked list, update cost
				next = &((*next)->next);
			}
		}
	}
}