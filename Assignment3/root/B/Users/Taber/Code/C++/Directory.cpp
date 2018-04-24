/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

class Directory {
public:
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
				subdirParent = subdirParent->getSubdir(subdirParentName);

				if (subdirParent == NULL)
					throw invalid_argument("Directory input defines subdirectory before parent directoriy.");

				path = path.substr(nextSlash + 1);
			}
			Directory *d = subdirParent->addDir(path);
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
				dir = dir->getSubdir(directoryName);

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

	// General constructor
	Directory(string name, Directory *parent) {
		this->name = name;
		this->parent = parent;
	}

	// Getters
	Directory *getParent() {
		return parent;
	}

	string getName() {
		return name;
	}

	bool isEmpty() {
		return files.size() == 0 && subdirs.size() == 0;
	}

	// Subdirectory functions
	Directory *addDir(string subdirName) {
		Directory *d = new Directory(subdirName, this);
		subdirs.push_back(d);

		return d;
	}

	Directory *getSubdir(string subdirName) {
		for (int i = 0; i < subdirs.size(); i++) {
			if (subdirs[i]->name == subdirName)
				return subdirs[i];
		}

		return NULL;
	}

	void deleteSubdir(string subdirName) {
		Directory *d = getSubdir(subdirName);

		if (d != NULL && d->isEmpty()) {
			int index = subdirs.size();
			for (int i = 0; i < index; i++) {
				if (subdirs[i]->name == d->name)
					index = i;
			}
			subdirs.erase(subdirs.begin() + index);
			delete d;
		}

	}

	// File functions
	File *addFile(string fileName, int size) {
		File *f = new File(fileName, size);
		files.push_back(f);

		return f;
	}

	File *getFile(string fileName) {
		for (int i = 0; i < files.size(); i++) {
			if (files[i]->name == fileName)
				return files[i];
		}

		return NULL;
	}

	void deleteFile(string fileName) {
		File *f = getFile(fileName);

		if (f != NULL) {
			int index = files.size();
			for (int i = 0; i < index; i++) {
				if (files[i]->name == f->name)
					index = i;
			}
			files.erase(files.begin() + index);
			delete f;
		}
	}

	// Print functions
	void dir() {
		queue<Directory*> dirs;
		dirs.push(this);

		while (!dirs.empty()) {
			Directory *d = dirs.front();
			cout << d->getPath() << endl;
			dirs.pop();

			for (int i = 0; i < d->subdirs.size(); i++) {
				dirs.push(d->subdirs[i]);
			}
		}

		
	}

	void ls() {
		cout << "Subdirectories:" << endl ;
		for (int i = 0; i < subdirs.size(); i++) {
			cout << subdirs[i]->name << endl;
		}

		cout << "\nFiles:" << endl;
		for (int i = 0; i < files.size(); i++) {
			cout << files[i]->toString() << endl;
		}
	}

	string getPath() {
		if (parent == NULL)
			return ".";
		else
			return parent->getPath() + "/" + name;
	}

private:
	string name;
	Directory *parent;
	vector<File*> files;
	vector<Directory*> subdirs;
};