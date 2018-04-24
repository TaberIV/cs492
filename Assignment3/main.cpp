/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"
#include "Directory.cpp"

// Functions for commands
void cd(vector<string> args) {
	
}

int main(int argc, char **args) {
	// Handles arguments---------------------------------------
	string fileListPath, dirListPath;
	int diskSize = 512, blockSize = 16;

	for (int i = 1; i < argc - 1; i++) {
		if (strcmp(args[i], "-f") == 0)
			fileListPath = args[++i];
		else if (strcmp(args[i], "-d") == 0)
			dirListPath = args[++i];
		else if (strcmp(args[i], "-s") == 0)
			diskSize = stoi(args[++i]);
		else if (strcmp(args[i], "-b") == 0)
			diskSize = stoi(args[++i]);
	}

	Directory *root = new Directory(fileListPath, dirListPath);
	//---------------------------------------------------------

	Directory *currentDir = root;
	cout << endl;

	bool exit = false;
	string input;
	while (!exit) {
		cout << currentDir->getPath() << "$ ";
		getline(cin, input);
		
		// Parse arguments
		while (input[0] == ' ')
			input = input.substr(1);

		string command = input.substr(0, input.find(' '));

		vector<string> args;
		while (input.find(' ') != string::npos) {
			input = input.substr(input.find(' ') + 1);

			if (input[0] == ' ' || input[0] == '\0')
				continue;

			if (input.find(' ') != string::npos)
				args.push_back(input.substr(0, input.find(' ')));
			else
				args.push_back(input);
		}

		string commands[] = {"cd", "cd..", "ls", "mkdir", "create", "append", "remove", "delete", "exit", "dir", "prfiles", "prdisk"};
		string commandFunctions[] = {

		}

		// Execute commands
	}

	// Dealocate data structures
}