
#include <stdio.h>
#include <stdlib.h>
#include "Solver.h"
#include "Printer.h"
#include "Cell.h"
#include "linkedList.h"


/*generate an empty board*/
Cell** createBoard(int n, int m)
{
	int i, j, k;
	Cell **board = (Cell**)malloc(n * m * sizeof(Cell*));
	if(!board)
	{
		faildMalloc();
	}
	for(i = 0; i < n*m; i++)
	{
		board[i] = (Cell*) malloc(n * m * sizeof(Cell));
		if(!board[i])
		{
			faildMalloc();
		}
	}
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			board[i][j].value = 0;
			board[i][j].row = i;
			board[i][j].column = j;
			board[i][j].fixed = 0;
			board[i][j].sizeOfPos = 0;
			board[i][j].valid = 0;
			board[i][j].possibles = (int*)malloc(n * m * sizeof(int));
			if(!board[i][j].possibles)
			{
				faildMalloc();
			}
			for(k = 0; k < n*m; k++)
			{
				board[i][j].possibles[k] = 0;
			}
		}
	}
	return board;
}

/*check if a value is possible in this row*/
int validRow(int row, int column, int z, int n, int m, Cell** board)
{
	int i;
	for (i = 0; i < (n * m); i++)
	{
		if (i != column)
		{
			if ((board[row][i].value) == z)
			{
				return 0;
			}
		}
	}
	return 1;
}

/*check if a value is possible in this column*/
int validColumn(int row, int column, int z, int n, int m, Cell** board)
{
	int i;
	for (i = 0; i < (n * m); i++)
	{
		if (i != row)
		{
			if ((board[i][column].value) == z)
			{
				return 0;
			}
		}
	}
	return 1;
}

/*check if a value is possible in this block*/
int validBlock(int row, int column, int z, int n, int m, Cell** board)
{
	int i, j, blockRow, blockColumn;
	blockRow = (row/m)*m;
	blockColumn = (column/n)*n;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if ((blockRow + i) != row && (blockColumn + j) != column)
			{
				if (board[blockRow + i][blockColumn + j].value == z)
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

/*check if a value is possible in this cell*/
int validNum(int row, int column, int z, int n, int m, Cell** board)
{
	if (z < 1 || z > (n * m))
	{
		return 0;
	}
	if ((validRow(row, column, z, n, m, board) == 1) && (validColumn(row, column, z, n, m, board) == 1)
			&& (validBlock(row, column, z, n, m, board) == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*check which values are possible in this cell*/
void possibleNums(int row, int column, int n, int m, Cell** board)
{
	int num, cnt = 0;
	for(num = 1; num <= n * m; num++)
	{
		if (validNum(row, column, num, n, m, board))
		{
			board[row][column].possibles[cnt] = num;
			cnt++;
		}
	}
	board[row][column].sizeOfPos = cnt;
}

void turnToFixed1(int n, int m, Cell** board)
{
	int i, j;
	for (i = 0; i < n * m; i++)
	{
		for (j = 0; j < n * m; j++)
		{
			if (board[i][j].value != 0)
			{
				board[i][j].fixed = 1;
			}
		}
	}
}

/*change each cell that holds value to fixed*/
void turnToFixed(int n, int m, Cell** board)
{
	int i, j;
	for (i = 0; i < n * m; i++)
	{
		for (j = 0; j < n * m; j++)
		{
			if (board[i][j].value != 0)
			{
				if (board[i][j].fixed == 1)
				{
					board[i][j].fixed = 1; /*original fixed*/
				}
				else
				{
					board[i][j].fixed = 2;
				}
			}
			else
			{
				board[i][j].fixed = 0;
			}
		}
	}
}

/*returns how many empty cells the current board contains*/
int emptyCells(int n, int m, Cell** board)
{
	int cnt = 0, i, j;
	for (i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			if(!board[i][j].value)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

/* checking if the board is erroneous */
int checkBoard(int n, int m, Cell** board)
{
	int i, j;
	for (i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			if(board[i][j].value)
			{
				if(!validNum(i, j, board[i][j].value, n, m, board))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

/*clear all not fixed cells*/
void eraseNotFix(int n, int m, Cell** board)
{
	int i, j;
	for (i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			if(board[i][j].fixed == 0)
			{
				board[i][j].value = 0;
			}
		}
	}
}

/* set value z to cell x,y */
int set(int* xyz, Cell** board, Cell**clone, int mode)
{
	int x = xyz[0], y = xyz[1], z = xyz[2];
	if(mode == 1) 		/* Edit mode */
	{
		board[y][x].value = z;
		clone[y][x].value = z;
		clone[y][x].fixed = board[y][x].fixed;
		return 1;				/* set successfully */
	}
	else 				/* Solve mode */
	{
		if(!board[y][x].fixed)
		{
			board[y][x].value = z;
			clone[y][x].value = z;
			clone[y][x].fixed = 0;
			return 1;			/* set successfully */
		}
		else
		{
			printf("Error: cell is fixed\n");
			return 0;			/* set unsuccessful */
		}
	}
}

/*check if the current board is solvable*/
int validate(int n, int m, Cell** board, Cell** solution)
{
	int flag = 0;
	flag = ilpSolver(n, m, board, solution);
	if (flag != 2)
	{
		return 0;
	}
	return 1;
}

/*return which value the user should set in this cell*/
void hint(int n, int m, int* xyz, Cell** board, Cell** solution)
{
	int flagHint;
	flagHint = ilpSolver(n, m, board, solution);
	if(flagHint == 0)
	{
		printf("Error during the build process or in the optimization of the model.\n");
	}
	else if(flagHint == 1)
	{
		printf("Error: the board is unsolvable.\n");
	}
	else if(flagHint == 2)
	{
		printf("Hint: set %d in cell <%d,%d>.\n", solution[xyz[1]][xyz[0]].value, xyz[0] + 1, xyz[1] + 1);
	}
}

/*free any memory that allocated to this board*/
void freeBoard(int n, int m, Cell** board)
{
	int i, j;
	for(i=0; i< n * m; i++)
	{
		for(j = 0; j < n*m ; j++)
		{
			free(board[i][j].possibles);
		}
		free(board[i]);
	}
	free(board);
}


/*copy values of origin board  to destination board
 *if any cell is fixed in origin,it will be fixed in destination*/
void cloneCopy(Cell** dest, Cell** orig, int n, int m)
{
	int i, j;
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			dest[i][j].value = orig[i][j].value;
			dest[i][j].fixed = orig[i][j].fixed;
		}
	}
}


/*start the game*/
void game(linkedList* reun)
{
	int flag = 0;
	printf("Hello and welcome to the best sudoku game!\n");
	printf("You are in Init mode, please choose solve or edit mode.\n");
	while(!flag)
	{
		printf("Please enter command:\n");
		flag = interpreter(reun);
	}
}

/*empty all cells from cell[i][j] to the last cell */
void zeroPoint(int n, int m, Cell** board, int i, int j)
{
	int k ,l;
	for (l = j; l < n*m; l++)
	{
		if (board[i][l].fixed != 1)
		{
			board[i][l].value = 0;
			board[i][l].fixed = 0;
		}
	}
	for (k = i + 1; k < n*m; k++)
	{
		for (l = 0; l < n*m; l++)
		{
			if (board[k][l].fixed != 1)
			{
				board[k][l].value = 0;
				board[k][l].fixed = 0;
			}
		}
	}
}

/*check the validity of all cells, if cell is erroneous, mark it as such*/
void markErrors(int n, int m, Cell** board)
{
	int i, j;
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			if(validNum(i, j, board[i][j].value, n, m, board))
			{
				board[i][j].valid = 1;
			}
			else
			{
				board[i][j].valid = 0;
			}
		}
	}
}

/*fill all obvious cells*/
void autoFill(int n, int m, Cell** board, Cell** clone, linkedList* reun)
{
	int i, j, flag = 1;
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			possibleNums(i, j, n, m, board);
		}
	}
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			if(!board[i][j].value)
			{
				if(board[i][j].sizeOfPos == 1)
				{
					addNode(reun,i, j, board[i][j].value, board[i][j].possibles[0], flag);
					if(flag)
					{
						flag = 2;
					}
					board[i][j].value = board[i][j].possibles[0];
					clone[i][j].value = board[i][j].possibles[0];
					printf("updated cell <%d,%d> to %d.\n", j + 1, i + 1,board[i][j].value);
				}
			}
		}
	}
}

/*count the number of empty cells in the board, and save an array of those cells*/
int howManyEmpty(int n, int m, Cell** board, next* emptyCells)
{
	int i, j, count = 0;
	next curr;
	for(i = 0; i < n * m; i++)
	{
		for(j = 0; j < n * m; j++)
		{
			if(board[i][j].value == 0)
			{
				curr.row = i;
				curr.column = j;
				emptyCells[count] = curr;
				count ++;
			}
		}
	}
	return count;
}

/*add generate's changes to the undo-redo list*/
void generateToReun(int n, int m, Cell** boardBef, Cell** boardAft, linkedList* reun)
{
	int i, j, flagCh = 1;
	for(i = 0; i < n * m; i++)
	{
		for(j = 0; j < n * m; j++)
		{
			if(boardBef[i][j].value != boardAft[i][j].value)
			{
				if(flagCh)
				{
					addNode(reun, i, j, boardBef[i][j].value, boardAft[i][j].value, 1);
				}
				else
				{
					addNode(reun, i, j, boardBef[i][j].value, boardAft[i][j].value, 2);
				}
			}
		}
	}
}

/*generate a board with y filled cells*/
void generate(int n, int m, int x, int y, Cell** board, Cell** clone, Cell** solution, linkedList* reun)
{
	int numtofill, numtoempty = 0, count = 0, i, j, place = -1, iter = 0, flag = -1, sp = 0;
	next curr;
	next* emptyCells = (next*)malloc(sizeof(next) * n * m * n * m);
	Cell** sol2 = createBoard(n, m);
	Cell** clone2 = createBoard(n,m);
	Cell** cloneForReun = createBoard(n,m);
	if(emptyCells == NULL)
	{
		faildMalloc();
	}
	cloneCopy(cloneForReun, board, n, m);
	count = howManyEmpty(n, m, board, emptyCells);
	if(count < x)
	{
		printf("There are no %d empty cells in the board.\n", x);
	}
	else if(y < 0 || y > n*m*n*m)
	{
		printf("Error: value of y is incorrect.\n");
	}
	else
	{
		while(iter < 1000)
		{
			numtofill = 0;
			while(numtofill < x)
			{
				place = rand() % count;
				curr = emptyCells[place];
				while(clone[curr.row][curr.column].value)
				{
					place = rand() % count;
					curr = emptyCells[place];
				}
				possibleNums(curr.row, curr.column, n, m, clone);
				if(!clone[curr.row][curr.column].sizeOfPos)
				{
					continue;
				}
				sp = (clone[curr.row][curr.column].sizeOfPos);
				clone[curr.row][curr.column].value = clone[curr.row][curr.column].possibles[rand() % sp];
				numtofill ++;
			}
			if(numtofill != x)
			{
				iter++;
				continue;
			}
			flag = ilpSolver(n,m,clone,sol2);
			if(flag == 2)
			{
				cloneCopy(solution, sol2,n, m);
				cloneCopy(board, sol2, n, m);
				break;
			}
			cloneCopy(clone, board, n, m);
			iter++;
		}
		if(flag != 2)
		{
			printf("Error: unable to generate, number of iterations passed 1000.\n");
		}
		else
		{
			while(numtoempty < y)
			{
				i = rand() % (n * m);
				j = rand() % (n * m);
				if(!clone2[i][j].value)
				{
					clone2[i][j].value = board[i][j].value;
					clone2[i][j].fixed = board[i][j].fixed;
					numtoempty ++;
				}
			}
			cloneCopy(board, clone2, n, m);
			cloneCopy(clone, board, n, m);
		}
	}
	generateToReun(n, m, cloneForReun, board, reun);
	freeBoard(n, m, sol2);
	freeBoard(n, m, clone2);
	freeBoard(n, m, cloneForReun);
	free(emptyCells);
}

void guess(int n, int m, float tresh, Cell** board, Cell** clone, linkedList* reun)
{
	int flag;
	if(!checkBoard(n, m, board))
	{
		erroneusBoard();
	}
	else
	{
		flag = lpSolver(n, m, tresh, board, clone, reun, -1, -1);
		if(flag == 0)
		{
			printf("Error during the built process or in the optimization of the model.\n");
		}
	}
}

void guessHint(int n, int m, Cell** board, Cell** clone, linkedList* reun, int x, int y)
{
	int flag;
	if(!checkBoard(n, m, board))
	{
		erroneusBoard();
	}
	else if(board[y][x].fixed == 1)
	{
		printf("Error: cell is fixed.\n");
	}
	else if(board[y][x].value)
	{
		printf("Error: cell already contains a value.\n");
	}
	else
	{
		flag = lpSolver(n, m, 0, board, clone, reun, x, y);
		if(flag == 0)
		{
			printf("Error during the build process or in the optimization of the model.\n");
		}
		else if(flag == 1)
		{
			unsolvableBoard();
		}
	}
}


