/*
 * Cell.c
 *
 *  Created on: 2 ���� 2019
 *      Author: shake
 */
#include <stdlib.h>

typedef struct{
	int value;
	int fixed;
	int *possibles;
	int sizeOfPos;
	int row;
	int column;
	int valid;
}Cell;


