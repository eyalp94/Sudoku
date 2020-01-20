/*
 * Solver.h
 *
 *  Created on: 29 במאי 2019
 *      Author: shake
 */

#ifndef SOLVER_H_
#define SOLVER_H_
#include "Parser.h"
#include "Game.h"
#include "Cell.h"
#include "stack.h"
#include "gurobi_c.h"
#include "linkedList.h"


/*return the next cell*/
next nextCell(int n, int m, int row, int column);

/*return the next not fixed cell*/
next nextNotFixed(int n, int m, Cell** board, int row, int column);

/* solve the board using backtracking algorithm*/
Stack* stackBacktracking(int n, int m, Cell** board);

/*return the number of possibles solutions of the current board*/
int exhaustiveBacktracking(int n, int m, Cell** board);

/* return sum of array */
int sumArr(int* arr, int length);

/*return how many variables we need */
int getNumOfVars(int n, int m, Cell** board, int* varsArr);

/* sum of cells from beginning to index */
int sumTo(int* arr, int inx);

/* return the index of the variable in the variable array that represents row i, column j and value k*/
int getPlaceOfVar(int* arr, int n, int m, Cell**board, int i, int j, int k);

/* solve the board using ILP */
int ilpSolver(int n, int m, Cell** board, Cell** solution);

/* solve the board using LP */
int lpSolver(int n, int m, double tresh, Cell** board, Cell** clone, linkedList* reun, int x, int y);

/* free all the malloced variables */
void freeGurobi(GRBenv* env, GRBmodel* model, int* varArr, double* sol, double* lb, char* vtype, int* ind, double* val);


#endif /* SOLVER_H_ */
