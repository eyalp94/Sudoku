/*
 * linkedList.c
 *
 *  Created on: 1 баев 2019
 *      Author: shake
 */
#include <stdlib.h>
#include <stdio.h>
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

Node* createNode(int row, int column, int bef, int aft, int change)
{
	Node* node = (Node*)malloc(sizeof(Node));
	if(node == NULL)
	{
		printf("Error: malloc has failed.\n");
		exit(0);
	}
	node->next = NULL;
	node->prev = NULL;
	node->row = row;
	node->column = column;
	node->bef = bef;
	node->aft = aft;
	node->change = change;
	return node;
}

linkedList* createList()
{
	linkedList* list = (linkedList*)malloc(sizeof(linkedList));
	if(list == NULL)
	{
		printf("Error: malloc has failed.\n");
		exit(0);
	}
	list->first = createNode(-1,-1,-1,-1, -1);
	list->last = createNode(-1,-1,-1,-1, -1);
	list->first->next = list->last;
	list->last->prev = list->first;
	list->curr = list->first;
	list->length = 0;
	return list;
}

void addNode(linkedList* list, int row, int column, int bef, int aft, int change)
{
	Node* newNode = createNode(row, column, bef, aft, change);
	if(!list->length)
	{
		list->first->next = newNode;
		newNode->prev = list->first;
	}
	else
	{
		newNode->prev = list->last->prev;
		newNode->prev->next = newNode;
	}
	newNode->next = list->last;
	list->last->prev = newNode;
	list->length++;
	list->curr = list->last->prev;
}

void deleteNode(linkedList* list, Node* node)
{
	Node* temp = node->next;
	list->length--;
	node->prev->next = temp;
	temp->prev = node->prev;
	free(node);
}

void deleteAll(linkedList* list)
{
	int i, len = list->length;
	for(i = 0; i < len; i++)
	{
		deleteNode(list, list->last->prev);
	}
}

void deleteFromHere(linkedList* list, Node* fromHere)
{
	Node* node = fromHere;
	while(fromHere != list->last)
	{
		fromHere = node->next;
		deleteNode(list, node);
		node = fromHere;
	}
}

void undo(linkedList* reun, Cell** board, Cell** clone, int UOR)	/*UOR = 0 for undo, 1 for reset*/
{
	int i, j;
	if(!(reun->curr->change))
	{
		i = reun->curr->row;
		j = reun->curr->column;
		board[i][j].value = reun->curr->bef;
		clone[i][j].value = reun->curr->bef;;
		reun->curr = reun->curr->prev;
		if(!UOR)
		{
			printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
		}
	}
	else
	{
		while(reun->curr->change != 1)
		{
			i = reun->curr->row;
			j = reun->curr->column;
			board[i][j].value = reun->curr->bef;
			clone[i][j].value = reun->curr->bef;;
			reun->curr = reun->curr->prev;
			if(!UOR)
			{
				printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
			}
		}
		i = reun->curr->row;
		j = reun->curr->column;
		board[i][j].value = reun->curr->bef;
		clone[i][j].value = reun->curr->bef;;
		reun->curr = reun->curr->prev;
		if(!UOR)
		{
			printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
		}
	}

}

void redo(linkedList* reun, Cell** board, Cell** clone)
{
	int i, j;
	reun->curr = reun->curr->next;
	if(!(reun->curr->change))
	{
		i = reun->curr->row;
		j = reun->curr->column;
		board[i][j].value = reun->curr->aft;
		clone[i][j].value = reun->curr->aft;
		printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
	}
	else
	{
		i = reun->curr->row;
		j = reun->curr->column;
		board[i][j].value = reun->curr->aft;
		clone[i][j].value = reun->curr->aft;
		reun->curr = reun->curr->next;
		printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
		while(reun->curr->change == 2)
		{
			i = reun->curr->row;
			j = reun->curr->column;
			board[i][j].value = reun->curr->aft;
			clone[i][j].value = reun->curr->aft;
			reun->curr = reun->curr->next;
			printf("Cell <%d,%d> changed to %d.\n", j+1, i+1, board[i][j].value);
		}
		reun->curr = reun->curr->prev;
	}

}

void freeList(linkedList* list)
{
	deleteAll(list);
	free(list->first);
	free(list->last);
	free(list);
}
