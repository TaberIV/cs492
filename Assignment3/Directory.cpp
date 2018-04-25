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
	time_t time_stamp;

	// Constructor for creating
	// hierarchicy from input files
	Directory(string fileListPath, string dirListPath) {
		// Initialize this directory
		name = "root";
		parent = NULL;
		updateTimeStamp();

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
			string subdirParentName;
			Directory *subdirParent = this;
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
			subdirParent->addDir(path);
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
			try {
				dir->addFile(path, fileSize);
			} catch (NoSpaceException &e) {
				cout << "Cannot create file '" + path + "': Not enough space" << endl;
			}

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
		updateTimeStamp();
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

	// Setters
	void updateTimeStamp() {
		time(&time_stamp);

		if (parent != NULL)
			parent->updateTimeStamp();
	}

	// Subdirectory functions
	Directory *addDir(string subdirName) {
		Directory *d = new Directory(subdirName, this);
		subdirs.push_back(d);
		updateTimeStamp();

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
		int index = subdirs.size();
		for (int i = 0; i < index; i++)
			if (subdirs[i]->name == subdirName)
				index = i;

		if (index != files.size()) {
			Directory *d = subdirs[index];
			subdirs.erase(subdirs.begin() + index);

			delete d;
			updateTimeStamp();
		}
	}

	// File functions
	File *addFile(string fileName, int size) {
		File *f = new File(fileName, size);

		if (f != NULL) {
			files.push_back(f);
			updateTimeStamp();
		}

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
		int index = files.size();
		for (int i = 0; i < index; i++)
			if (files[i]->name == fileName)
				index = i;

		if (index != files.size()) {
			File *f = files[index];
			files.erase(files.begin() + index);

			f->remove(f->size);
			delete f;
			updateTimeStamp();
		}
	}

	int getFrag() {
		int frag = 0;

		for (int i = 0; i < files.size(); i++)
			frag += files[i]->getFrag();

		for (int i = 0; i < subdirs.size(); i++)
			frag += subdirs[i]->getFrag();

		return frag;
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

	void prfiles() {
		for (int i = 0; i < files.size(); i++)
			cout << files[i]->prfile(getPath()) <<  endl;

		for (int i = 0; i < subdirs.size(); i++)
			subdirs[i]->prfiles();
	}

	string toString() {
		string sizeString = to_string(getSize());
		string timeString = ctime(&time_stamp);
		timeString.replace(timeString.length() - 1, 1, " ");

		return timeString + " " + sizeString + "\t" + name;
	}

	int getSize() {
		int size = 0;

		for (int i = 0; i < files.size(); i++)
			size += files[i]->size;

		for (int i = 0; i < subdirs.size(); i++)
			size += subdirs[i]->getSize();

		return size;
	}

	void ls() {
		cout << "Subdirectories:" << endl ;
		for (int i = 0; i < subdirs.size(); i++) {
			cout << subdirs[i]->toString() << endl;
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