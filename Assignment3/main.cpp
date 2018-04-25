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
		string arg = args[i];
		if (arg.compare("-f") == 0)
			fileListPath = args[++i];
		else if (arg.compare("-d") == 0)
			dirListPath = args[++i];
		else if (arg.compare("-s") == 0)
			diskSize = stoi(args[++i]);
		else if (arg.compare("-b") == 0)
			blockSize = stoi(args[++i]);
	}

	// Initialize global Disk Blocks
	Ldisk = new DiskData(diskSize, blockSize);

	// Define directory tree
	Directory *root = new Directory(fileListPath, dirListPath);
	Directory *currDir = root;

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
		enum Command {cd, cdUp, ls, mkdir, create, append, remove, delete_, exit_, dir, prfiles, prdisk, none};
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
		Directory *d;
		File *f;
		int bytes;
		if (!error) {
			switch (command) {
				// Change directory
				case cd:
					d = currDir->getSubdir(args[0]);
					
					if (d == NULL) {
						error = true;
						errorMsg += args[0] + ": No such directory";
					}
					else
						currDir = d;

					break;
				case cdUp:
					d = currDir->getParent();

					if (d != NULL)
						currDir = d;

					break;
				case ls:
					currDir->ls();
					break;
				// Access files/folders
				case mkdir:
					if (currDir->getSubdir(args[0]) != NULL) {
						error = true;
						errorMsg += "cannot create directory '" + args[0] + "': File exists";
					}
					else
						currDir->addDir(args[0]);

					break;
				case create:
					if (currDir->getFile(args[0]) != NULL) {
						error = true;
						errorMsg += "cannot create file '" + args[0] + "': File exists";
					}
					else {
						f = currDir->addFile(args[0], 0);
					}

					break;
				case append:
					currDir->updateTimeStamp();
					f = currDir->getFile(args[0]);
					bytes = stoi(args[1]);

					if (f == NULL) {
						error = true;
						errorMsg +=  args[0] + ": No such file";
					} else if (bytes <= 0) {
						error = true;
						errorMsg += "bytes to append must be positive.";
					} else {
						try {
							f->append(bytes);
						} catch (NoSpaceException &e) {
							error = true;
							errorMsg += "Out of space";
						}
					}

					break;
				case remove:
					f = currDir->getFile(args[0]);
					bytes = stoi(args[1]);

					if (f == NULL) {
						error = true;
						errorMsg += args[0] + "': No such file";
					} else if (bytes <= 0) {
						error = true;
						errorMsg += "bytes to remove must be positive.";
					} else if (bytes > f->size) {
						error = true;
						errorMsg += "cannot remove more than number of bytes in file.";
					}else
						f->remove(bytes);

					break;
				case delete_:
					d = currDir->getSubdir(args[0]);
					f = currDir->getFile(args[0]);

					if (d == NULL && f == NULL) {
						error = true;
						errorMsg += "cannot delete '" + args[0] + "': No such file or directory";
					} else if (f != NULL) {
						currDir->deleteFile(args[0]);
					} else if (d != NULL && !d->isEmpty()) {
						error = true;
						errorMsg += "cannot delete '" + args[0] + "': Directory is not empty";
					} else if (d != NULL) {
						currDir->deleteSubdir(args[0]);
					}

					break;
				// File system info
				case exit_:
					exiting = true;
					break;
				case dir:
					root->dir();
					break;
				case prfiles:
					root->prfiles();
					break;
				case prdisk:
					Ldisk->prdisk();
					cout << "Fragmentation: " << root->getFrag() << endl;
					break;
				default:
					error = true;
					errorMsg += "This should not be here";
			}
		}

		if (error)
			cout << errorMsg << endl;
	}

	// Dealocate data structures
}