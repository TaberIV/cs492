/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"

struct Page{
	int pageNum;
	int validBit;
	int LTA;
};

class Process {
private:
	int page_size;
	vector<Page> pageTable;

public:
	int page_offset;

	Process(int pID, int numMemLocs, int page_size, int page_offset) {
		this->page_offset = page_offset;
		this->page_size = page_size;
		
		// Create Page Table
		int numPages = numMemLocs / page_size;
		
		if (numMemLocs % page_size != 0)
			numPages++;

		for (int i = 0; i < numPages; i++) {
			Page p;
			p.pageNum = i + page_offset;
			p.validBit = 0;
			p.LTA = 0;

			pageTable.push_back(p);
		}
	}

	int getPageOffset() {
		return page_offset;
	}

	int getMemoryOffset() {
		return page_offset * page_size;
	}

	int accessMemLoc(int memLoc, int counter) {
		Page p = pageTable[(memLoc / page_size) - page_offset];
		
		int validBit = p.validBit;
		p.validBit = 1;
		p.LTA = counter;
		pageTable[(memLoc / page_size) - page_offset] = p;

		return validBit;
	}

	int getGreatestPageNum() {
		return page_offset + pageTable.size();
	}

	void swapOut(int memLoc) {
		pageTable[(memLoc / page_size) - page_offset].validBit = 0;
	}
};