/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"


int main(int argc, char** args) {
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

	/* Debug variable
	if (argc > num_args + 1)
		debug = stoi(args[num_args + 1]) == 1;
	else
		debug = false;*/
	
	// Print args
	fflush(stdout);
	printf("Page size: %d\n", page_size);
	printf("Page replacement Algorithm: %s\n", page_algm == FIFO ? "FIFO" : page_algm == Clock ? "Clock" : "LRU");
	printf("Pre-paging: %s\n", pre_page ? "On" : "Off");

	
}
