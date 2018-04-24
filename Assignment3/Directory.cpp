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
	Directory(string fileListPath, string dirListPath) {
		// Initialize this directory
		name = "root";
		parent = NULL;

		// Setup reading from files
		ifstream dirList(dirListPath);
		ifstream fileList(fileListPath);
		int maxStrLen = 256;
		char dirPath[maxStrLen];
		char fileInfo[maxStrLen];

		// Check that the input files exist
		if (!dirList.good() || !fileList.good())
			throw invalid_argument("Must provide valid files for -f and -d.\n");

		// Create directories
		dirList.getline(dirPath, maxStrLen); // Skips ./
		dirList.getline(dirPath, maxStrLen);
		while (!dirList.eof()) {
			Directory *subdirParent = this;
			string subdirParentName = "root";
			string path = dirPath;
			int nextSlash;

			// Removes ./
			nextSlash = path.find('/');
			path = path.substr(nextSlash + 1); 
			while ((nextSlash = path.find('/')) != string::npos) {
				subdirParentName = path.substr(0, nextSlash);
				subdirParent = subdirParent->getSubdirectory(subdirParentName);

				if (subdirParent == NULL)
					throw invalid_argument("Directory input defines subdirectory before parent directoriy.");

				path = path.substr(nextSlash + 1);
			}
			subdirParent->addDirectory(path);
			dirList.getline(dirPath, maxStrLen);
		}

		// Create files
		fileList.getline(fileInfo, maxStrLen);
		while (!fileList.eof()) {
			string fileInfo2 = fileInfo;

			// Cut out uneeded info
			fileInfo2 = fileInfo2.substr(46); 
			while (fileInfo2[0] == ' ')
				fileInfo2 = fileInfo2.substr(1);

			// Get size
			int fileSize = stoi(fileInfo2.substr(0, fileInfo2.find(' ')));


			// Go down path to create file in correct directory
			string path = fileInfo2.substr(fileInfo2.find('/') + 1);
			Directory *dir = this;
			string directoryName;
			int nextSlash;
			while ((nextSlash = path.find('/')) != string::npos) {
				directoryName = path.substr(0, nextSlash);
				dir = dir->getSubdirectory(directoryName);

				if (dir == NULL)
					throw invalid_argument("File belongs to nonexistant directory.");

				path = path.substr(nextSlash + 1);
			}
			dir->addFile(path, fileSize);

			fileList.getline(fileInfo, maxStrLen);
		}

		// Close files
		dirList.close();
		fileList.close();
	}

	Directory(string name, Directory *parent) {
		this->name = name;
		this->parent = parent;
	}

	void addFile(string fileName, int size) {
		File *f = new File(fileName, size);
		files.push_back(*f);
	}

	void addFile(string fileName) {
		File *f = new File(fileName);
		files.push_back(*f);
	}

	void addDirectory(string subdirName) {
		Directory *d = new Directory(subdirName, this);
		subdirs.push_back(*d);
	}

	Directory *getSubdirectory(string subdirName) {
		for (int i = 0; i < subdirs.size(); i++) {
			if (subdirs[i].name == subdirName)
				return &subdirs[i];
		}

		return NULL;
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

	string getPath() {
		if (parent == NULL)
			return name + "/";
		else
			return parent->getPath() + name + "/";
	}

private:
	Directory *parent;
	vector<File> files;
	vector<Directory> subdirs;
};