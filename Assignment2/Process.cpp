/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"
#include "PageTable.cpp"

class Process {
private:
	int page_offset;

public:
	int pID;
	PageTable *pageTable;

	Process(int pID, int numMemLocs, int page_size, int page_offset) {
		this->pID = pID;
		this->page_offset = page_offset;
		
		// Create page Table
		pageTable = new PageTable(numMemLocs, page_size, page_offset);
	}

	int getPageOffset() {
		return page_offset;
	}

	int accessMemLoc(int memLoc, int counter) {
		return pageTable->accessMemLoc(memLoc, counter);
	}
};