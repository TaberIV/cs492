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

	for (int i = 0; i < numPages; i++) {
		int page[3] = {i + page_offset, 0, 0};
		table.push_back(page);
	}
}

	void accessMemLoc(int memLoc, int counter) {
		table[(memLoc / page_size) - page_offset][1] = 1;
		table[(memLoc / page_size) - page_offset][2] = counter;
	}
};