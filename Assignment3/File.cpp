/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

class File {
public:
	string name;
	int size;
	time_t time_stamp;
	Block *startBlock;

	File(string name, int size) {
		this->name = name;
		this->size = size;
		time(&time_stamp);

		// Allocate memory in linked list
		startBlock = new Block(size);
	}

	void append(int bytes) {			
		size += bytes;
		time(&time_stamp);

		// Allocate memory in linked list
	}

	void remove(int bytes) {			
		size -= bytes;
		time(&time_stamp);

		// Deallocate memory in linked list
	}

	string toString() {
		string sizeString = to_string(size);
		string timeString = ctime(&time_stamp);
		timeString.replace(timeString.length() - 1, 1, " ");

		return timeString + " " + sizeString + "\t" + name;
	}

	string prfile() {
		string ret = toString();

		ret += "\t| ";

		vector<int> addresses;
		startBlock->getBlockAddresses(addresses);

		for (int i = 0; i < addresses.size(); i++)
			ret += to_string(addresses[i]) + " ";

		return ret;
	}

	int getFrag() {
		if (size % Ldisk->blockSize == 0)
			return 0;
		else
			return Ldisk->blockSize - (size % Ldisk->blockSize);
	}
};