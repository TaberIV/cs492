/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

class DiskData {
public:
	int blockSize;

	// Constructor initializing linked list
	DiskData(int diskSize, int blockSize) {
		this->blockSize = blockSize;
		int numBlocks = diskSize / blockSize;

		next = NULL;

		firstBlock = 0;
		lastBlock = numBlocks - 1;

		free = true;
	}

	int getBlock() {
		if (free) {
			int freeBlock = firstBlock;

			if (prev == NULL) {
				DiskData *nextBlock = new DiskData(firstBlock + 1, lastBlock);

				if (next != NULL)
					next->prev = nextBlock;

				nextBlock->next = next;
				nextBlock->prev = this;
				next = nextBlock;

				next->firstBlock = firstBlock + 1;
				next->lastBlock = lastBlock;

				lastBlock = firstBlock;
				free = false;
			} else {
				prev->lastBlock = firstBlock;

				if (firstBlock != lastBlock)
					firstBlock += 1;
				else if (next == NULL) {
					prev->next = NULL;

					delete this;
				} else {
					prev->next = next->next;
					prev->next->prev = prev;
					prev->lastBlock = next->lastBlock;

					delete next;
					delete this;
				}
			}
			
			return freeBlock;
		}
		else if (next != NULL)
			return next->getBlock();
		else
			throw NoSpaceException();
	}

	bool hasSpace(int bytes) {
		if (free)
			bytes = bytes - getNumBlocks() * blockSize;

		if (bytes <= 0)
			return true;
		else if (next != NULL)
			return next->hasSpace(bytes);
		else
			return false;
	}

	void prdisk() {
		cout << (free ? "Free: " : "In use: ");
		cout << firstBlock << "-" << lastBlock << endl;

		if (next != NULL)
			next->prdisk();
	}

private:
	int getNumBlocks() {
		return lastBlock - firstBlock + 1;
	}

	DiskData *next, *prev;
	int firstBlock;
	int lastBlock;
	bool free = true;
};