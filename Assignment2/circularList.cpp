/***********************************************
 * CS492 - Operating Systems
 * Assignment 2
 *
 * Authors: E. Taber McFarlin and Matt Swentzel
 * We pledge our honor that we have abided by 
 * the Stevens Honor System
***********************************************/

#include "assign2.h"

class circularList {
public:
	circularList(int data) {
		Node newNode = new Node(data);
		
		return newNode;
	}

	addAfter(struct Node **prev, int data) {
		Node newNode = new Node(data);

		newNode->next = prev->next;
		prev->next = newNode;
		
		newNode->prev = prev;
		newNode->next->prev = newNode;
	}

	getNext(struct Node *node) {
		return node->next;
	}

	getPrev(struct Node *node) {
		return node->prev;
	}


private:
	class Node {
	public:
		int data;
		Node *next, *prev;

		Node (int data) {
			this->data = data;
		}
	}
}