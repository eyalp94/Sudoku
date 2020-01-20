
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Parser.h"
#include "Game.h"
#include "Cell.h"
#include "Printer.h"
#include "SPBufferset.h"
#include "stack.h"
#include "gurobi_c.h"
#include "linkedList.h"


/*return the next cell
 *
 * never call the function on last cell
 *
 */
next nextCell(int n, int m, int row, int column)
{
	next RC ;
	if (column < (n*m - 1))
	{
		RC.row = row;
		RC.column = column + 1;
	}
	else
	{
		RC.row = row + 1;
		RC.column = 0;
	}
	return RC;
}

void deleteFirst(Cell** board, int n, int m, int row, int column)
{
	int i;
	for(i = 0; i < (n * m - 1); i++)
	{
		board[row][column].possibles[i] = board[row][column].possibles[i + 1];
	}
	board[row][column].sizeOfPos--;
}

/*return the next not fixed cell
 *
 * never call the function on last cell
 *
 */
next nextNotFixed(int n, int m, Cell** board, int row, int column)
{
	next RC;
	RC = nextCell(n, m, row, column);
	if(board[RC.row][RC.column].fixed)
	{
		while(board[RC.row][RC.column].fixed)
		{
			if((RC.row == n * m - 1) && (RC.column == n * m - 1))
			{
				return RC;
			}
			RC = nextCell(n, m, RC.row, RC.column);
		}
	}
	else
	{
		return nextCell(n, m, row, column);
	}
	return RC;
}

/* solve the board using backtracking algorithm*/
Stack* stackBacktracking(int n, int m, Cell** board)
{
	int *possibles, sizeOfPos = 0, row = 0, column = 0, firstRow, firstColumn;
	next RC;
	Cell* temp;
	Stack* stack = createStack(n*m*n*m);
	if((row == 0 && column == 0) && board[0][0].fixed)
	{
		RC = nextNotFixed(n,m,board,row,column);
		row = RC.row;
		column = RC.column;
	}
	firstRow = row;
	firstColumn = column;
	/* Get all valid values for the cell*/
	possibleNums(row, column, n, m, board);
	possibles = board[row][column].possibles; /* the possible values*/
	sizeOfPos = board[row][column].sizeOfPos;
	board[row][column].value = possibles[0];
	push(stack, board[row][column]);
	RC = nextNotFixed(n, m, board, row, column); /* Next cell indices*/
	row = RC.row;
	column = RC.column;
	possibleNums(row, column, n, m, board);
	possibles = board[row][column].possibles; /* the possible values*/
	sizeOfPos = board[row][column].sizeOfPos;

	while(!(isEmpty(stack) && board[firstRow][firstColumn].sizeOfPos <= 0))
	{
		while (sizeOfPos > 0)
		{
			board[row][column].value = possibles[0];
			push(stack, board[row][column]);
			RC = nextNotFixed(n, m, board, row, column); /* Next cell indices*/
			row = RC.row;
			column = RC.column;
			possibleNums(row, column, n, m, board);
			possibles = board[row][column].possibles; /* the possible values*/
			sizeOfPos = board[row][column].sizeOfPos;
			if((row == n * m - 1) && (column == n * m - 1))
			{
				if(board[row][column].fixed)
				{
					return stack;
				}
				else
				{
					if (sizeOfPos != 0)
					{
						board[row][column].value = possibles[0];
						push(stack, board[row][column]);
						return stack;
					}
				}
			}
		}
		/* In case could not further complete the board:*/
		temp = pop(stack);
		row = temp->row;
		column = temp->column;
		sizeOfPos = temp->sizeOfPos;
		/*		deleteOne(board, row, column, 0, sizeOfPos);  Remove value*/
		deleteFirst(board, n, m, row, column);
		board[row][column].value = 0;
		possibles = board[row][column].possibles;
		sizeOfPos = board[row][column].sizeOfPos;
	}
	freeStack(stack);
	return NULL;
}

/*return the number of possibles solutions of the current board*/
int exhaustiveBacktracking(int n, int m, Cell** board)
{
	int cnt = 0, row, column, sizeOfPos;
	/*Cell** clone = createBoard(n, m);*/
	Cell* temp;
	Stack* stack, *tempStack;
	stack = stackBacktracking(n, m, board);
	if (stack == NULL)
	{
		return 0;
	}
	cnt++;
	/*cloneCopy(clone, board, n, m);  clone is original solution*/
	while(!isEmpty(stack))
	{
		temp = pop(stack);
		row = temp->row;
		column = temp->column;
		sizeOfPos = temp->sizeOfPos;
		/*		cloneCopy(board, clone, n, m);*/
		zeroPoint(n, m, board, row,column);
		turnToFixed(n, m, board);
		while (sizeOfPos > 0)
		{
			/*			deleteOne(board, temp->row, temp->column, 0, temp->sizeOfPos);*/
			deleteFirst(board, n, m, temp->row, temp->column);
			sizeOfPos--;
			if (board[row][column].sizeOfPos > 0)
			{
				board[row][column].value = board[row][column].possibles[0];
				push(stack, board[row][column]);
				board[row][column].fixed = 2;
				tempStack = stackBacktracking(n, m, board);
				if (tempStack != NULL)
				{
					cnt++;
					concat(stack,tempStack);
					freeStack(tempStack);
					break;
				}
			}
		}
	}
	freeStack(stack);
	/*freeBoard(n, m, clone);*/
	return cnt;
}

/* return sum of array */
int sumArr(int* arr, int length)
{
	int sum = 0, i;
	for(i = 0; i < length; i++)
	{
		sum += arr[i];
	}
	return sum;
}

/*return how many variables we need */
int getNumOfVars(int n, int m, Cell** board, int* varsArr)
{
	int i, j, place = 0, dim = n*m;

	for(i = 0; i < dim; i++)
	{
		for(j = 0; j < dim; j++)
		{
			if(board[i][j].value != 0)
			{
				varsArr[place] = 1;
			}
			else
			{
				possibleNums(i, j, n, m, board);
				varsArr[place] = board[i][j].sizeOfPos;
			}
			place++;
		}
	}
	return sumArr(varsArr, dim * dim);
}

/* sum of cells from beginning to index */
int sumTo(int* arr, int inx)
{
	return sumArr(arr, inx) + arr[inx];
}

/* return the index of the variable in the variable array that represents row i, column j and value k*/
int getPlaceOfVar(int* arr, int n, int m, Cell**board, int i, int j, int k)
{
	int dim = n * m, count = 0, l;
	if(board[i][j].value != 0) /* there is a value in the cell*/
	{
		if(board[i][j].value != k)
		{
			return -1;
		}
		return (sumTo(arr, i * dim + j) - 1);
	}
	else /* the cell is empty */
	{
		if(!validNum(i, j, k, n, m, board))
		{
			return -1;
		}

		for(l = k + 1; l <= dim; l++)
		{
			if(validNum(i, j, l, n, m, board))
			{
				count ++;
			}

		}
		return (sumTo(arr, i * dim + j) - count - 1);
	}
}

/* free all the memory allocated to gurobi functions */
void freeGurobi(GRBenv* env, GRBmodel* model, int* varArr, double* sol, double* lb, char* vtype, int* ind, double* val)
{
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(lb);
	free(sol);
	free(vtype);
	free(varArr);
	free(ind);
	free(val);
}

void freeGurobilp(GRBenv* env, GRBmodel* model, int* varArr, double* sol, double* lb, char* vtype, int* ind, double* val, double* obj)
{
	freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
	free(obj);
}

/* solve the board using ILP */
int ilpSolver(int n, int m, Cell** board, Cell** solution)
{
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;
	int error = 0, dim = n*m, i, j, k, place = -1, countPlace = 0, bi, bj, optimalstat;
	int flag = 2; /* 0 - an error occurred during set up of the linear program, 1- board unsolvable, 2 - ILP good, solution set*/
	double objval;
	int* varArr = (int*)malloc(sizeof(int) * dim * dim);		/* save how many possibilities there are for values for var*/
	int numOfVars = getNumOfVars(n, m, board, varArr); 				/* save how many variables we need*/
	double* sol = (double*)malloc(sizeof(double) * numOfVars);
	int* ind = (int*)malloc(sizeof(int) * numOfVars);
	double* val = (double*)malloc(sizeof(double) * numOfVars);
	double* lb = (double*)malloc(sizeof(double) * numOfVars);
	char* vtype = (char*)malloc(numOfVars);
	if(varArr == NULL || sol == NULL || ind == NULL || val == NULL || lb == NULL || vtype == NULL)
	{
		faildMalloc();
	}
	cloneCopy(solution, board, n, m);

	/* Create an empty model */

	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			for (k = 1; k <= dim; k++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j , k);
				if(place > -1)
				{
					if(board[i][j].value == k)
					{
						lb[place] = 1;
					}
					else
					{
						lb[place] = 0;
					}
					vtype[place] = GRB_BINARY;
				}

			}
		}
	}

	/* Create environment */

	error = GRBloadenv(&env, "sudoku.log");
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 0;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 0;
	}

	/* Create new model */

	error = GRBnewmodel(env, &model, "sudoku", numOfVars, NULL, lb, NULL, vtype, NULL );
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 0;
	}

	/* Each cell gets a value */

	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			countPlace = 0;
			for (k = 1; k <= dim; k++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
				return 0;
			}
		}
	}

	/* each value appears once in each row */

	for (k = 1; k <= dim; k++)
	{
		for (i = 0; i < dim; i++)
		{
			countPlace = 0;
			for (j = 0; j < dim; j++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
				return 0;
			}
		}
	}

	/* each value appears once in each column */

	for (k = 1; k <= dim; k++)
	{
		for (j = 0; j < dim; j++)
		{
			countPlace = 0;
			for (i = 0; i < dim; i++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
				return 0;
			}
		}
	}

	/*each value appears once in every block */
	for (k = 1; k <= dim; k++)
	{
		for (bi = 0; bi < n; bi++)
		{
			for (bj = 0; bj < m; bj++)
			{
				countPlace = 0;
				for (i = bi * m; i < (bi + 1) * m; i++)
				{
					for (j = bj * n; j < (bj + 1) * n; j++)
					{
						place = getPlaceOfVar(varArr, n, m, board, i, j, k);

						if(place > -1)
						{
							ind[countPlace] = place;
							val[countPlace] = 1.0;
							countPlace++;
						}
					}
				}

				error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error)
				{
					freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
					return 0;
				}
			}
		}
	}

	/* Optimize model */

	error = GRBoptimize(model);
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 0;
	}

	/* Write model to 'sudoku.lp' */

	error = GRBwrite(model, "sudoku.lp");
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 0;
	}

	/* Capture solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimalstat);
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 1;
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 1;
	}

	/*get the solution*/

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, numOfVars, sol);
	if (error)
	{
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 1;
	}

	/* solution found */
	if (optimalstat == GRB_OPTIMAL) {
		flag = 2;

	}
	/* no solution found */
	else if (optimalstat == GRB_INF_OR_UNBD) {
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 1;
	}
	/* error or calculation stopped */
	else {
		freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
		return 1;
	}

	/* create the board */

	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			for (k = 1; k <= dim; k++)
			{
				countPlace = getPlaceOfVar(varArr, n, m, board, i, j, k);
				if(countPlace != -1)
				{
					if(sol[countPlace] == 1.0)
					{
						solution[i][j].value = k;
					}

				}
			}
		}
	}

	freeGurobi(env, model, varArr, sol, lb, vtype, ind, val);
	return flag;


}

/* solve the board using LP */
int lpSolver(int n, int m, float tresh, Cell** board, Cell** clone, linkedList* reun, int x, int y)
{
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;
	int error = 0, dim = n*m, i, j, k, place = -1, countPlace = 0, bi, bj, optimalstat, tmp, first ,cntCan, chosen;
	int flag = 2, flagReun = 0; /* 0 - an error occurred during set up of the linear program, 1- board unsolvable, 2 - ILP good, solution set*/
	double objval;
	float rndNUM , p, sum;
	int* candidtes = (int*)malloc(sizeof(int) * dim);
	int* varArr = (int*)malloc(sizeof(int) * dim * dim);		/* save how many possibilities there are for values for var*/
	int numOfVars = getNumOfVars(n, m, board, varArr); 				/* save how many variables we need*/
	double* sol = (double*)malloc(sizeof(double) * numOfVars);
	int* ind = (int*)malloc(sizeof(int) * numOfVars);
	double* val = (double*)malloc(sizeof(double) * numOfVars);
	double* lb = (double*)malloc(sizeof(double) * numOfVars);
	char* vtype = (char*)malloc(numOfVars);
	double* obj = (double*)malloc(sizeof(double) * numOfVars);
	if(candidtes == NULL || varArr == NULL || sol == NULL || ind == NULL || val == NULL || lb == NULL || vtype == NULL || obj == NULL)
	{
		faildMalloc();
	}
	/* Create environment */

	error = GRBloadenv(&env, "sudoku.log");
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Create empty model */

	error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL );
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Add variables */

	/* coefficients and variable types */
	for(i = 0; i < numOfVars; i++)
	{
		obj[i] = (1+ rand()% dim);
		vtype[i] = GRB_CONTINUOUS;
	}

	/* add variables to model */
	error = GRBaddvars(model, numOfVars, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
	if (error) {
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Each cell gets a value */

	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			countPlace = 0;
			for (k = 1; k <= dim; k++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
				return 0;
			}
		}
	}

	/* each value appears once in each row */

	for (k = 1; k <= dim; k++)
	{
		for (i = 0; i < dim; i++)
		{
			countPlace = 0;
			for (j = 0; j < dim; j++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
				return 0;
			}
		}
	}

	/* each value appears once in each column */

	for (k = 1; k <= dim; k++)
	{
		for (j = 0; j < dim; j++)
		{
			countPlace = 0;
			for (i = 0; i < dim; i++)
			{
				place = getPlaceOfVar(varArr, n, m, board, i, j, k);

				if(place > -1)
				{
					ind[countPlace] = place;
					val[countPlace] = 1.0;
					countPlace ++;
				}
			}
			error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error)
			{
				freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
				return 0;
			}
		}
	}

	/*each value appears once in every block */
	for (k = 1; k <= dim; k++)
	{
		for (bi = 0; bi < n; bi++)
		{
			for (bj = 0; bj < m; bj++)
			{
				countPlace = 0;
				for (i = bi * m; i < (bi + 1) * m; i++)
				{
					for (j = bj * n; j < (bj + 1) * n; j++)
					{
						place = getPlaceOfVar(varArr, n, m, board, i, j, k);

						if(place > -1)
						{
							ind[countPlace] = place;
							val[countPlace] = 1.0;
							countPlace++;
						}
					}
				}

				error = GRBaddconstr(model, countPlace, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error)
				{
					freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
					return 0;
				}
			}
		}
	}

	/* Optimize model */

	error = GRBoptimize(model);
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Write model to 'sudoku.lp' */

	error = GRBwrite(model, "sudoku.lp");
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 0;
	}

	/* Capture solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimalstat);
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 1;
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 1;
	}

	/*get the solution*/

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, numOfVars, sol);
	if (error)
	{
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 1;
	}

	/* solution found */
	if (optimalstat == GRB_OPTIMAL)
	{
		flag = 2;
	}
	/* no solution found */
	else if (optimalstat == GRB_INF_OR_UNBD) {
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 1;
	}
	/* error or calculation stopped */
	else {
		freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
		return 1;
	}

	/* create the board */
	if(x == -1 && y == -1)
	{
		for (i = 0; i < dim; i++)
		{
			for (j = 0; j < dim; j++)
			{
				if(board[i][j].value != 0)
					continue;
				p = 0;
				sum = 0;
				for (k = 1; k <= dim; k++)
				{
					tmp = getPlaceOfVar(varArr, n, m, board, i, j, k);
					if(tmp > -1)
					{
						if(sol[tmp] >= tresh && validNum(i, j, k, n, m, board))
						{
							p += sol[tmp];
						}
					}
				}
				if(p == 0)
				{
					cntCan = 0;
					for (k = 1; k <= dim; k++)
					{
						tmp = getPlaceOfVar(varArr, n, m, board, i, j, k);
						if(tmp > -1)
						{
							if(validNum(i, j, k, n, m, board))
							{
								candidtes[cntCan] = k;
								cntCan++;
							}
						}
					}
					if(cntCan)
					{
						chosen = candidtes[rand() % cntCan]; /*choose randomly if threshold is 0*/
						printf("Guess: the value of cell <%d,%d> changed to %d.\n",j+1 ,i+1 ,chosen);
						if(flagReun == 0)
						{
							flagReun = 1;
							addNode(reun, i, j, board[i][j].value, chosen, 1);
						}
						else
						{
							addNode(reun, i, j, board[i][j].value, chosen, 2);
						}
						board[i][j].value = chosen;
						clone[i][j].value = chosen;
					}

				}
				else
				{
					rndNUM = (float)rand()/((float)(RAND_MAX) + 1) ;
					for (k = 1; k <= dim; k++)
					{
						tmp = getPlaceOfVar(varArr, n, m, board, i, j, k);
						if(tmp > -1)
						{
							if(sol[tmp]/p >= tresh && validNum(i, j, k, n, m, board))
							{
								if(sum < rndNUM && sum + sol[tmp]/p >= rndNUM)
								{
									printf("Guess: the value of cell <%d,%d> changed to %d.\n",j+1 ,i+1 ,k);
									if(flagReun == 0)
									{
										flagReun = 1;
										addNode(reun, i, j, board[i][j].value, k, 1);
									}
									else
									{
										addNode(reun, i, j, board[i][j].value, k, 2);
									}
									board[i][j].value = k;
									clone[i][j].value = k;
									break;
								}
								sum = sum+sol[tmp];
							}
						}
					}
				}
			}
		}
	}
	else
	{
		first = 1;
		for (k = 1; k <= dim; k++)
		{
			tmp = getPlaceOfVar(varArr, n, m, board, x, y, k);
			if(tmp > -1)
			{
				if(sol[tmp] > 0)
				{
					if(first)
					{
						printf("Guesses for cell <%d,%d> are: value = %d, score = %f", y + 1, x + 1, k, sol[tmp]);
						first = 0;
					}
					else{
						printf(",value = %d, score = %f", k, sol[tmp]);
					}
				}
			}
		}
		if(!first)
			printf(". \n");
		else
		{
			printf("no legal guesses for this cell.\n");
			flag = 3;
		}
	}
	free(candidtes);
	freeGurobilp(env, model, varArr, sol, lb, vtype, ind, val, obj);
	return flag;


}






