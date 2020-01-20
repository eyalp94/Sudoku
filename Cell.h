/*
 * Cell.h
 *
 *  Created on: 2 αιεπ 2019
 *      Author: shake
 */

#ifndef CELL_H_
#define CELL_H_

typedef struct{
	int value;
	int fixed;
	int *possibles;
	int sizeOfPos;
	int row;
	int column;
	int valid;
}Cell;

typedef struct{
	int row;
	int column;

}next;



#endif /* CELL_H_ */
