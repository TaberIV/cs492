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
	Algorithm page_algm;
	if (strcmp(args[2], "FIFO") == 0)
		page_algm = FIFO;
	else if (strcmp(args[2], "LRU") == 0)
		page_algm = LRU;
	else if (strcmp(args[2], "Clock") == 0)
		page_algm = Clock;
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
	printf("Page replacement Algorithm: %s\n", page_algm == FIFO ? "FIFO" : page_algm == Clock ? "Clock" : "LRU");
	printf("Pre-paging: %s\n\n", pre_page ? "On" : "Off");
	//---------------------------------------------------------

	// Create Processes from plist*****************************
	printf("Creating Processes...\n");
	vector<Process*> processes;
	ifstream plist("InputFiles/plist.txt");

	int i = 0, page_offset = 0;
	char pID[32], numMemLocs[32];

	plist.getline(pID, 32, ' ');
	plist.getline(numMemLocs, 32);
	while (!plist.eof()) {
		processes.push_back(new Process(stoi(pID), stoi(numMemLocs), page_size, page_offset));
		page_offset += stoi(numMemLocs);
		
		plist.getline(pID, 32, ' ');
		plist.getline(numMemLocs, 32);
		i++;
	}

	printf("Processes created!\n\n");
	//*********************************************************

	//Pre-loading memory---------------------------------------
	const int memoryAmount = 512;
	const int memoryPerProcess = memoryAmount / processes.size();
	const int memoryPages = memoryAmount / page_size;
	int memory[memoryPages];

	printf("Memory Available: %d\n", memoryAmount);
	printf("Memory Per Process: %d\n", memoryPerProcess);
	printf("Max pages in Memory: %d\n", memoryPages);

	// For each process
	for (int pID = 0; pID < processes.size(); pID++) {
		printf("\nPre-loading process %d\n", pID);
		// While there is room add pages in process' memory do so
		int memoryOffset = pID * memoryPerProcess / page_size;
		int pageNum = 0;
		while(memoryOffset + pageNum * page_size <= memoryPerProcess) {
			printf("accessMemLoc: %d\n", memoryOffset + pageNum * page_size);
			processes[pID]->accessMemLoc(memoryOffset + pageNum * page_size, 0);
			printf("Loc1\n");
			memory[memoryOffset + pageNum] = page_offset / page_size;
			
			printf("Page %d loaded into memory %d\n", memoryOffset + pageNum, memoryOffset + pageNum);

			pageNum++;
		}
	}
	//---------------------------------------------------------

	//Following ptrace*****************************************
	//*********************************************************
}
