/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"
#include "Process.cpp"

int findPageforPrepage(int pageNum, vector<int> &memory, int maxPage) {
	if (pageNum >= maxPage)
		return -1;

	for (int i = 0; i < memory.size(); i++) {
		if (memory[i] == pageNum + 1)
			return findPageforPrepage(pageNum + 1, memory, maxPage);
	}

	return pageNum + 1;
}

//FIFO Swapping Algorithm
void swapFIFO(int pageNum, vector<int> &memory, int regionStart, int regionEnd){
	memory.erase(memory.begin() + regionStart);
	memory.insert((memory.begin() + regionEnd), pageNum);
}

//LRU Swapping Algorithm
void swapLRU(int pageNum, vector<int> &memory, int regionStart, int regionEnd){

}

//Clock Swapping Algorithm
void swapClock(int pageNum, vector<int> &memory, int regionStart, int regionEnd){

}


void swapIn(int pageNum, vector<int> &memory, int regionStart, int regionEnd, Algorithm page_alg) {
	switch (page_alg) {
		case FIFO:
			swapFIFO(pageNum, memory, regionStart, regionEnd);
			break;
		case LRU:
			swapLRU(pageNum, memory, regionStart, regionEnd);
			break;
		case Clock:
			swapClock(pageNum, memory, regionStart, regionEnd);
			break;
		default: 
			printf("Invalid Algorithm\n");
	}	
}

int main(int argc, char** args) {
	// Handle command line arguments---------------------------
	// Ensure proper number of args
	const int num_args = 3;
	if (argc < num_args + 1)
		throw invalid_argument("Wrong number of arguments, should be 3.\n");

	// Parse and initialize arg variables
	// Page Size
	int page_size = stoi(args[1]);
	
	// Page Replacement Algorithm
	Algorithm page_alg;
	if (strcmp(args[2], "FIFO") == 0)
		page_alg = FIFO;
	else if (strcmp(args[2], "LRU") == 0)
		page_alg = LRU;
	else if (strcmp(args[2], "Clock") == 0)
		page_alg = Clock;
	else
		throw invalid_argument("Invalid Page Replacement Algorithm. Valid choices: FIFO, LRU, Clock.\n");

	// Pre-paging
	bool pre_page;
	if (strcmp(args[3], "+") == 0)
		pre_page  = true;
	else if (strcmp(args[3], "-") == 0)
		pre_page = false;
	else
		throw invalid_argument("Invalid entry for pre-paging, must be + or -\n");
	
	// Print args
	fflush(stdout);
	printf("Page size: %d\n", page_size);
	printf("Page replacement Algorithm: %s\n", page_alg == FIFO ? "FIFO" : page_alg == Clock ? "Clock" : "LRU");
	printf("Pre-paging: %s\n\n", pre_page ? "On" : "Off");
	//---------------------------------------------------------

	// Create Processes from plist*****************************
	printf("Creating Processes...\n");
	vector<Process*> processes;
	ifstream plist("InputFiles/plist.txt");

	int i = 0, page_offset = 0, pID, numMemLocs;
	char pID_str[32], numMemLocs_str[32];

	plist.getline(pID_str, 32, ' ');
	plist.getline(numMemLocs_str, 32);
	while (!plist.eof()) {
		pID = stoi(pID_str);
		numMemLocs = stoi(numMemLocs_str);

		processes.push_back(new Process(pID, numMemLocs, page_size, page_offset));
		page_offset += numMemLocs / page_size;
		if (numMemLocs % page_size != 0)
			page_offset++;

		printf("Page Offset: %d\n", page_offset);

		plist.getline(pID_str, 32, ' ');
		plist.getline(numMemLocs_str, 32);
		i++;
	}

	printf("Processes created!\n\n");
	//*********************************************************

	//Pre-loading memory---------------------------------------
	const int memoryAmount = 512;
	const int memoryPerProcess = memoryAmount / processes.size();
	const int pagesPerProcess = memoryPerProcess / page_size;
	const int totalPages = pagesPerProcess * processes.size();
	vector<int> memory; // An array of the pages in main memory

	printf("Memory Available: %d\n", memoryAmount);
	printf("Memory Per Process: %d\n", memoryPerProcess);
	printf("Pages In Memory Per Process: %d\n\n", pagesPerProcess);

	// For each process
	for (int pID = 0; pID < processes.size(); pID++) {
		printf("Pre-loading process %d\n", pID);
		// While there is room add pages in process' memory do so
		int memoryIndex = pID * pagesPerProcess;
		int pageNum = processes[pID]->getPageOffset();
		int processPages = 0;
		while(processPages < pagesPerProcess) {
			int validBit = processes[pID]->accessMemLoc(pageNum * page_size, 0);

			memory.insert(memory.begin() + memoryIndex, pageNum);

			pageNum++;
			memoryIndex++;
			processPages++;
		}
	}

	// int printStart = 0, printEnd = memory.size();
	// for (int i = printStart; i < printEnd; i++) {
	// 	printf("memory[%d] = %d\n", i, memory[i]);
	// }
	//---------------------------------------------------------

	//Following ptrace*****************************************
	printf("\nFollowing ptrace.txt\n");
	unsigned /*long*/ counter = 0;
	int swapCounter;
	
	ifstream ptrace("InputFiles/ptrace.txt");

	char memLoc_str[32];
	int memLoc, validBit;

	ptrace.getline(pID_str, 32, ' ');
	ptrace.getline(memLoc_str, 32);
	while (!ptrace.eof()) {
		pID = stoi(pID_str);
		memLoc = stoi(memLoc_str);
		int pageNum = memLoc / page_size + processes[pID]->getPageOffset();

		validBit = processes[pID]->accessMemLoc(memLoc + processes[pID]->getMemoryOffset(), counter);


		// Do a page swap
		if (validBit == 0) {
			swapCounter++;

			int regionStart = pagesPerProcess * pID;
			int regionEnd = pagesPerProcess * (pID + 1) - 1;

			swapIn(pageNum, memory, regionStart, regionEnd, page_alg);

			if (pre_page) {
				int maxPage = processes[pID]->getGreatestPageNum();
				int prePageNum = findPageforPrepage(pageNum, memory, maxPage);

				if (prePageNum > 0) {
					swapIn(prePageNum, memory, regionStart, regionEnd, page_alg);
				}
			}
		}

		counter++;
		ptrace.getline(pID_str, 32, ' ');
		ptrace.getline(memLoc_str, 32);
	}
	//*********************************************************

	printf("%d\n", swapCounter);
}