/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"

class PageTable	{
private:
	vector<int*> table;
	int page_size;
	int page_offset;

public:
	PageTable(int numMemLocs, int page_size, int page_offset) {
	this->page_size = page_size;
	this->page_offset = page_offset;
	int numPages = numMemLocs / page_size;
	
	if (numMemLocs % page_size != 0)
		numPages++;

	printf("Memory Locations: %d\n", numMemLocs);
	for (int i = 0; i < numPages; i++) {
		int page[3] = {i + page_offset, 0, 0};

		table.push_back(page);

		int *row;
		row = table[i];

		printf("table[%d] = [%d, %d, %d]\n", i, row[0], row[1], row[2]);
	}
}

	int accessMemLoc(int memLoc, int counter) {
		int *row;
		printf("table[%d] = ", (memLoc / page_size) - page_offset);
		row = table[(memLoc / page_size) - page_offset];

		printf("[%d, %d, %d]\n", row[0], row[1], row[2]);

		// table[(memLoc / page_size) - page_offset][1] = 1;
		// table[(memLoc / page_size) - page_offset][2] = counter;

		int validBit = 1;
		return validBit;
	}

	void swapOut(int memLoc) {
		table[(memLoc / page_size) - page_offset][1] = 0;
	}
};