/*
 * linkedList.h
 *
 *  Created on: 1 баев 2019
 *      Author: shake
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include "Cell.h"

typedef struct Node{
	struct Node* next;
	struct Node* prev;
	int row;
	int column;
	int bef;
	int aft;
	int change; /*0 - only one cell have changed, 1 - the first cell in a series of changed cells,
	 	 	 	  2 - not the first cell in a series of changed cells, */
}Node;

typedef struct{
	int length;
	Node* first;
	Node* last;
	Node* curr;
}linkedList;

Node* createNode(int row, int column, int bef, int aft, int change);
linkedList* createList();
void addNode(linkedList* list, int row, int column, int bef, int aft, int change);
void deleteNode(linkedList* list, Node* node);
void deleteAll(linkedList* list);
void deleteFromHere(linkedList* list, Node* fromHere);
void undo(linkedList* reun, Cell** board, Cell** clone, int UOR);
void redo(linkedList* reun, Cell** board, Cell** clone);
void freeList(linkedList* list);

#endif /* LINKEDLIST_H_ */
