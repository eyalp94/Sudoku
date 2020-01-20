/*
 * Game.h
 *
 *  Created on: 28 במאי 2019
 *      Author: shake
 */

#ifndef GAME_H_
#define GAME_H_
#include "Cell.h"
#include "linkedList.h"



/*generate an empty board*/
Cell** createBoard(int n, int m);

/*check if a value is possible in this row*/
int validRow(int row, int column, int z, int n, int m, Cell** board);
/*check if a value is possible in this column*/
int validColumn(int row, int column, int z, int n, int m, Cell** board);
/*check if a value is possible in this block*/
int validBlock(int row, int column, int z, int n, int m, Cell** board);
/*check if a value is possible in this cell*/
int validNum(int row, int column, int z, int n, int m, Cell** board);

/*check which values are possible in this cell*/
void possibleNums(int row, int column, int n, int m, Cell** board);

/*change each cell that holds value to fixed*/
void turnToFixed1(int n, int m, Cell** board);
void turnToFixed(int n, int m, Cell** board);

/*returns the number of empty cells in the current board*/
int emptyCells(int n, int m, Cell** board);

/* checking if the board is erroneous */
int checkBoard(int n, int m, Cell** board);

/*clear all not fixed cells*/
void eraseNotFix(int n, int m, Cell** board);

/* set value z to cell x,y */
int set(int* xyz, Cell** board, Cell**clone, int mode);

/*return which value the user should set in this cell*/
void hint(int n, int m, int* xyz, Cell** board, Cell** solution);

/*check if the current board is solvable*/
int validate(int n, int m, Cell** board, Cell** solution);

/*free any memory that allocated to this board*/
void freeBoard(int n, int m, Cell** board);

/*copy values of origin board  to destination board
 *if any cell is fixed in origin,it will be fixed in destination*/
void cloneCopy(Cell** dest, Cell** orig, int n, int m);

/*start the game*/
void game(linkedList* reun);

/*empty all cells from cell[i][j] to the last cell */
void zeroPoint(int n, int m, Cell** board, int i, int j);

/*check the validity of all cells, if cell is erroneous, mark it as such*/
void markErrors(int n, int m, Cell** board);

/*fill all obvious cells*/
void autoFill(int n, int m, Cell** board, Cell** clone, linkedList* reun);

/*return the number of empty cells in the board, and save an array of those cells*/
int howManyEmpty(int n, int m, Cell** board, next* emptyCells);

/*add generate's changes to the undo/redo list*/
void generateToReun(int n, int m, Cell** boardBef, Cell** boardAft, linkedList* reun);

/*generate a board with y filled cells*/
void generate(int n, int m, int x, int y, Cell** board, Cell** clone, Cell** solution, linkedList* reun);
void guess(int n, int m, float tresh, Cell** board, Cell** clone, linkedList* reun);
void guessHint(int n, int m, Cell** board, Cell** clone, linkedList* reun, int x, int y);


#endif /* GAME_H_ */
