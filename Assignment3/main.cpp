/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

int main(int argc, char **args) {
	// Handle command line arguments
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

	// Define directory tree
	Directory *root = new Directory(fileListPath, dirListPath);
	Directory *currDir = root;

	Directory *A = root->getSubdir("A");
	Directory *C = A->getSubdir("C");

	// Accept user commands
	cout << endl;
	string input;
	bool exiting = false;
	while (!exiting) {
		cout << currDir->getPath() << "$ ";
		getline(cin, input);
		
		// Parse command and arguments
		while (input[0] == ' ')
			input = input.substr(1);

		string commandStr = input.substr(0, input.find(' '));
		if (commandStr.compare("") == 0) {
			continue;
		}

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

		// Error checking
		enum Command {cd, cdUp, ls, mkdir, create, append, remove_, delete_, exit_, dir, prfiles, prdisk, none};
		string commands[] = {"cd", "cd..", "ls", "mkdir", "create", "append", "remove", "delete", "exit", "dir", "prfiles", "prdisk"};
		int commandArgs[] = {1, 0, 0, 1, 1, 2, 2, 1, 0, 0, 0, 0};

		Command command = none;
		for (int i = 0; i < command; i++) {
			if (commandStr.compare(commands[i]) == 0)
				command = (Command) i;
		}

		bool error = false;
		string errorMsg = commandStr + ": ";
		if (command == none) {
			errorMsg += "command not found";
			error = true;
		}
		else if (commandArgs[command] != args.size()) {
			errorMsg += "wrong number of arguments";
			error = true;
		}

		// Execute command
		Directory *newDir;
		File *f;
		if (!error) {
			switch (command) {
				case cd:
					newDir = currDir->getSubdir(args[0]);
					
					if (newDir != NULL)
						currDir = newDir;
					else {
						error = true;
						errorMsg += args[0] + ": No such directory";
					}
					break;
				case cdUp:
					newDir = currDir->getParent();
					if (newDir != NULL)
						currDir = newDir;
					break;
				case ls:
					currDir->ls();
					break;
				case mkdir:
					if (currDir->getSubdir(args[0]) == NULL)
						currDir->addDir(args[0]);
					else {
						error = true;
						errorMsg += "cannot create directory '" + args[0] + "': File exists";
					}
					break;
				case create:
					if (currDir->getFile(args[0]) == NULL)
						currDir->addFile(args[0], 0);
					else {
						error = true;
						errorMsg += "cannot create file '" + args[0] + "': File exists";
					}
					break;
				case append:
					f = currDir->getFile(args[0]);
					if (f != NULL)
						f->append(stoi(args[1]));
					else {
						error = true;
						errorMsg += args[0] + "': No such file";
					}
					break;
				case remove_:
					break;
				case delete_:
					break;
				case exit_:
					break;
				case dir:
					break;
				case prfiles:
					break;
				case prdisk:
					break;
				default:
					error = true;
					cout << "This should not be here" << endl;
			}
		}

		if (error)
			cout << errorMsg << endl;
	}

	// Dealocate data structures
}