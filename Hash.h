#include <iostream>

#ifndef HASH_HEADER
#define HASH_HEADER

// Hash Table object
struct Ht_item {
	int key;				// item key stored as integer
	std::string state;		// state stored as string
	int cost;				// heuristic cost stored as integer
	Ht_item* next;			// next pointer used for linked list structure
};

// Hash Table class to be used for reached
class Hash {
private:
	Ht_item** states;		// array of hash table items
	int size;				// size of Hash Table
	int count;				// number of items within Hash Table 
	int hashFunc(std::string state);		// function to apply hash to get key
public:
	Hash();					// default constructor
	Hash(int s);
	~Hash();				// destructor
	void insert(std::string state, int cost);		// function to insert item into Hash Table
	bool search(std::string state);					// function to search item within Hash Table
	int getCost(std::string state);					// function to return item cost for item within Hash Table
	void setCost(std::string state, int cost);		// function to set item cost for item within Hash Table
};

#endif