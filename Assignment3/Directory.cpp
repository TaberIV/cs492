/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"
#include "File.cpp"

class Directory {
public:
	string name;

	// Constructor for creating
	// hierarchicy from input files
	Directory(string dirListPath, string fileListPath) {
		name = "root";
		parent = NULL;

		fstream dirList(dirListPath);
		string dir = "";

		while (!dirList.eof() && dir != "\n") {
			dir = dirList.getLine();
			printf("%s\n", dir);
		}
	}

	Directory(string name, Directory *parent) {
		this->name = name;
		this->parent = parent;
	}

	void addFile(string fileName) {
		File *f = new File(fileName);
		files.push_back(*f);
	}

	void addDirectory(string subdirName) {
		Directory *d = new Directory(subdirName, this);
		subdirs.push_back(*d);
	}

	string toString() {
		return name;
	}

	void ls() {
		cout << "Subdirectories:" << endl ;
		for (int i = 0; i < subdirs.size(); i++) {
			cout << subdirs[i].toString() << endl;
		}

		cout << "\nFiles:" << endl;
		for (int i = 0; i < files.size(); i++) {
			cout << files[i].toString() << endl;
		}
	}

	void cd(string dirName) {
		for (int i = 0; i < subdirs.size(); i++) {

		}
	}

private:
	Directory *parent;
	vector<File> files;
	vector<Directory> subdirs;
};