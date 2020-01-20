/*
 * files.h
 *
 *  Created on: 30 αιεμ 2019
 *      Author: shake
 */

#ifndef FILES_H_
#define FILES_H_
#include <stdio.h>
#include <stdlib.h>


/*returns the length of the longest line in the file*/
int maxLineLength(char* fileName);

/*checks whether the string represent a number*/
int isNum(char* str);

/*fills an empty board according to the file*/
int boardFromFile(FILE* fp, int m, int n, int ml,  Cell** board, int mode);

/*reads the file and returns the board that was in it*/
Cell** readFile(char* fileName, int* nm, int mode);

/*saves the current board to a file*/
int saveBoard(int n, int m, Cell** board, char* path);

#endif /* FILES_H_ */
