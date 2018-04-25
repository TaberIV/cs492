/***********************************************
 * CS492 - Operating Systems
 * Assignment 3
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#ifndef ASSIGN3_H
#define ASSIGN3_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <time.h>

using namespace std;

// Define out of space exception
struct NoSpaceException : public exception {
	const char * what () const throw () {
		return "Not enough space";
	}
};

#ifndef DISKDATA
#define DISKDATA
#include "DiskData.cpp"

// Define global Disk Blocks
DiskData *Ldisk;
#endif

#ifndef BLOCK
#define BLOCK
#include "Block.cpp"
#endif

#ifndef FILE
#define FILE
#include "File.cpp"
#endif

#ifndef DIRECTORY
#define DIRECTORY
#include "Directory.cpp"
#endif

#endif