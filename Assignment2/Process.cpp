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
	int R;
};

class Process {
private:
	int pID, page_size;
	vector<Page> pageTable;

public:
	int page_offset;

	Process(int pID, int numMemLocs, int page_size, int page_offset) {
		this->pID = pID;
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
		p.R = 1;

		pageTable[(memLoc / page_size) - page_offset] = p;

		return validBit;
	}

	int getGreatestPageNum() {
		return page_offset + pageTable.size();
	}

	void swapOut(int pageNum) {
		Page p = pageTable[pageNum - page_offset];

		p.validBit = 0;

		pageTable[pageNum - page_offset] = p;
	}

	int getCounter(int pageNum) {
		return pageTable[pageNum - page_offset].LTA;
	}

	int tickR(int pageNum) {
		Page p = pageTable[pageNum - page_offset];
		
		int R = p.R;
		p.R = 0;
		pageTable[pageNum - page_offset] = p;

		return R;
	}

	int getPID() {
		return pID;
	}

	void prepage(int pageNum) {
		Page p = pageTable[pageNum - page_offset];

		p.validBit = 1;
		p.R = 1;

		pageTable[pageNum - page_offset] = p;
	}
};