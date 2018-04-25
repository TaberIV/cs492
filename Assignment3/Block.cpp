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
	}

	void getBlockAddresses(vector<int> &addresses) {
		addresses.push_back(blockNum);
		
		if (next != NULL)
			next->getBlockAddresses(addresses);
	}

	void appendBlocks(int bytes) {
		if (next != NULL)
			next->appendBlocks(bytes);
		else
			next = new Block(bytes);
	}
};