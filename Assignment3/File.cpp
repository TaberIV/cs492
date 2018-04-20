/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign3.h"

class File {
public:
	string name;
	int size;
	time_t time_stamp;

	File(string name) {
		this->name = name;
		size = 0;
		time(&time_stamp);
	}

	string toString() {
		string sizeString = to_string(size);
		string timeString = ctime(&time_stamp);
		timeString.replace(timeString.length() - 1, 1, " ");

		return sizeString + " " + timeString + name;
	}
};