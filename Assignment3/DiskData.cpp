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
		prev = NULL;

		firstBlock = 0;
		lastBlock = numBlocks - 1;

		free = true;
	}

	DiskData(int firstBlock, int lastBlock, bool free, int blockSize) {
		this->firstBlock = firstBlock;
		this->lastBlock = lastBlock;
		this->free = free;
		this->blockSize = blockSize;

		next = NULL;
		prev = NULL;
	}

	~DiskData() {
		if (next != NULL)
			delete next;
	}

	void mergeHT() {
		if (next == NULL) {
			prev->next = NULL;
			prev->lastBlock = lastBlock;
			delete this;
		}
		else if (prev == NULL) {
			free = next->free;
			lastBlock = next->lastBlock;

			DiskData *nextPointer = next;
			next = next->next;
			delete nextPointer;

			next->prev = this;
		} else {
			prev->next = next->next;
			if (prev->next != NULL)
				prev->next->prev = prev;
			
			prev->lastBlock = next->lastBlock;

			next->next = NULL;
			delete this;
		}
	}

	void splitFromTail() {
		DiskData *nextBlock = new DiskData(firstBlock + 1, lastBlock, !free, blockSize);
		lastBlock = firstBlock;

		if (next != NULL)
			next->prev = nextBlock;

		nextBlock->next = next;
		nextBlock->prev = this;
		next = nextBlock;

		next->free = free;
		free = !free;
	}

	void splitOffTail() {
		DiskData *prevBlock = new DiskData(firstBlock, lastBlock - 1, free, blockSize);
		firstBlock = lastBlock;
		free = !free;

		if (prev != NULL)
			prev->next = prevBlock;

		prevBlock->prev = prev;
		prevBlock->next = this;
		prev = prevBlock;

	}

	void splitOffHead() {
		firstBlock++;
		prev->lastBlock++;
	}

	int getBlock() {
		if (free) {
			int openBlock = firstBlock;

			if (firstBlock == lastBlock) {
				mergeHT();
			} else if (prev == NULL) {
				splitFromTail();
			} else {
				splitOffHead();
			}
			
			return openBlock;
		}
		else if (next != NULL)
			return next->getBlock();
		else
			throw NoSpaceException();
	}

	int freeBlock(int blockNum) {
		if (blockNum > lastBlock) {
			next->freeBlock(blockNum);
		} else {
			if (free)
				cout << "Already freed" << endl;

			if (firstBlock == lastBlock) {
				mergeHT();
			} else if (blockNum == firstBlock) {
				if (prev != NULL) {
					splitOffHead();
				} else {
					splitFromTail();
				}
			} else if (blockNum == lastBlock) {
				if (next != NULL) {
					lastBlock--;
					next->firstBlock--;
				} else if (prev != NULL)
					splitOffTail();
				else {
					DiskData *nextBlock = new DiskData(lastBlock, lastBlock, !free, blockSize);
					nextBlock->prev = this;
					next = nextBlock;
					
					firstBlock = 0;
					lastBlock = lastBlock - 1;
				}
			} else {
				DiskData *nextBlock = new DiskData(blockNum, blockNum, !free, blockSize);
				DiskData *nextNextBlock = new DiskData(blockNum + 1, lastBlock, free, blockSize);
				lastBlock = blockNum - 1;

				if (next != NULL)
					next->prev = nextNextBlock;

				nextBlock->prev = this;
				nextBlock->next = nextNextBlock;
				nextNextBlock->next = next;
				nextNextBlock->prev = nextBlock;
				next = nextBlock;
			}
		}
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