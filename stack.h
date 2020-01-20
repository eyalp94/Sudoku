/*
 * stack.h
 *
 *  Created on: 30 αιεμ 2019
 *      Author: shake
 */

#ifndef STACK_H_
#define STACK_H_
#include "Cell.h"

typedef struct {
    int top;
    int capacity;
    Cell* array;
}Stack;

/*generate an empty stack*/
Stack* createStack(int capacity);

/*check whether the stack is empty*/
int isEmpty(Stack* stack);

/*check whether the stack is full*/
int isFull(Stack* stack);

/*add item to the top of the stack*/
void push(Stack* stack, Cell item);

/*remove item from the top of the stack*/
Cell* pop(Stack* stack);

/*return the item from the top of the stack*/
Cell* peek(Stack* stack);

/*free all memories allocated to the stack*/
void freeStack(Stack* stack);

/*concatenate another stack to the top of the current stack*/
void concat(Stack* stack, Stack* another);
#endif /* STACK_H_ */
