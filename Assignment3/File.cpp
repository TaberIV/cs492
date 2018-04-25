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
		if (!Ldisk->hasSpace(size))
			throw NoSpaceException();

		this->name = name;
		this->size = size;
		time(&time_stamp);

		startBlock = new Block(size);
	}

	void append(int bytes) {		
		if (bytes > getFrag()) {
			if (!Ldisk->hasSpace(bytes - getFrag()))
				throw NoSpaceException();
			else
				startBlock->appendBytes(bytes - getFrag());
		}

		size += bytes;
		time(&time_stamp);
	}

	void remove(int bytes) {
		time(&time_stamp);

		int bytesInLastBlock = Ldisk->blockSize - getFrag();
		
		if (bytes > bytesInLastBlock)
			startBlock->removeBytes(bytes);

		size -= bytes;
	}

	string toString() {
		string sizeString = to_string(size);
		string timeString = ctime(&time_stamp);
		timeString.replace(timeString.length() - 1, 1, " ");

		return timeString + " " + sizeString + "\t" + name;
	}

	string prfile(string path) {
		string ret = "";

		// Timestamp
		string timeString = ctime(&time_stamp);
		timeString.replace(timeString.length() - 1, 1, "  ");
		ret += timeString;

		// Path name and size
		ret += to_string(size) + "\t";
		ret += path + "/" + name ;

		// Memory info
		while (ret.length() < 60)
			ret += ' ';
		ret += "| ";
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