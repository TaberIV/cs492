/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

class Block {
public:
	int blockNum;
	Block *next;

	Block(int bytes) {
		blockNum = Ldisk->getBlock();
		
		if (bytes - Ldisk->blockSize > 0)
			next = new Block(bytes - Ldisk->blockSize);
		else
			next = NULL;
	}

	void appendBytes(int bytes) {
		if (next != NULL)
			next->appendBytes(bytes);
		else
			next = new Block(bytes);
	}

	int removeBytes(int bytes) {
		if (next != NULL) {
			bytes = next->removeBytes(bytes);
		
			if (bytes > Ldisk->blockSize) {
				bytes = bytes - Ldisk->blockSize;
				delete next;
				next = NULL;
			}
		}

		return bytes;
	}

	void getBlockAddresses(vector<int> &addresses) {
		addresses.push_back(blockNum);
		
		if (next != NULL) {
			next->getBlockAddresses(addresses);
		}
	}
};