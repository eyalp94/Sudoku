/*
 * stack.c
 *
 *  Created on: 30 αιεμ 2019
 *      Author: shake
 */

#include <stdlib.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Cell.h"


typedef struct {
	int top;
	int capacity;
	Cell* array;
}Stack;

/*generate an empty stack*/
Stack* createStack(int capacity)
{
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if(stack == NULL)
	{
		printf("Error: malloc has failed.\n");
		exit(0);
	}
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (Cell*)malloc(stack->capacity * sizeof(Cell));
	if(stack->array ==  NULL)
	{
		printf("Error: malloc has failed.\n");
		exit(0);
	}
	return stack;
}

/*check whether the stack is empty*/
int isEmpty(Stack* stack)
{
	return stack->top == -1;
}

/*check whether the stack is full*/
int isFull(Stack* stack)
{
	return stack->top == stack->capacity - 1;
}

/*add item to the top of the stack*/
void push(Stack* stack, Cell item)
{
	if (isFull(stack))
		return;
	stack->top++;
	stack->array[stack->top] = item;
}

/*remove item from the top of the stack*/
Cell* pop(Stack* stack)
{
	if (isEmpty(stack))
		return NULL;
	return &(stack->array[stack->top--]);
}

/*return the item from the top of the stack*/
Cell* peek(Stack* stack)
{
	if (isEmpty(stack))
		return NULL;
	return &(stack->array[stack->top]);
}

/*free all memories allocated to the stack*/
void freeStack(Stack* stack)
{
	free(stack->array);
	free(stack);
}

/*concatenate another stack to the top of the current stack*/
void concat(Stack* stack, Stack* another)
{
	int num = another->top, i;
	Stack* tempSt = createStack(stack->capacity);
	for (i = 0; i <= num; i++)
	{
		push(tempSt, *pop(another));
	}
	for (i = 0; i <= num; i++)
	{
		push(stack, *pop(tempSt));
	}
	freeStack(tempSt);
}

