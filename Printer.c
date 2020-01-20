/*
 * Printer.c
 *
 *  Created on: 29 במאי 2019
 *      Author: shake
 */
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "Cell.h"
#include "stack.h"
#include "SPBufferset.h"
#include "linkedList.h"


void prinExit(int n, int m, Cell** board, Cell** solution, Cell** clone, linkedList* reun)
{
	printf("Hope you enjoyed the game:)\nExiting...\n");
	if(board != NULL)
		freeBoard(n, m, board);
	if(solution != NULL)
		freeBoard(n, m, solution);
	if(clone != NULL)
		freeBoard(n, m, clone);
	if(reun != NULL)
		freeList(reun);
	exit(0);
}


void printBoard(int n, int m, Cell** board, int mark_errors)
{
	int i, j, k;
	markErrors(n, m, board);
	for (i = 0; i < m*n; i++)
	{
		if (i%(m) == 0)
		{
			for (k = 0; k < (4*m*n + m + 1); k++)
			{
				printf("-");
			}
			printf("\n");
		}
		for (j = 0; j < m*n; j++)
		{
			if (j%(n) == 0)
			{
				printf("|");
			}
			if (board[i][j].fixed)
			{
				printf(" %2d." ,board[i][j].value);
			}
			if (!board[i][j].fixed)
			{
				if (board[i][j].value == 0)
				{
					printf("    ");
				}
				else if(!board[i][j].valid && mark_errors)
				{
					printf(" %2d*" ,board[i][j].value);
				}
				else
				{
					printf(" %2d ", board[i][j].value);
				}
			}
			if (j == m*n-1)
			{
				printf("|");
			}
		}
		printf("\n");
	}
	for (k = 0; k < (4*m*n + m + 1); k++)
	{
		printf("-");
	}
	printf("\n");
}

void prinExitFile(FILE* fp, Cell** board, int n, int m, char* line)
{
	fclose(fp);
	if(board != NULL)
		freeBoard(n, m, board);
	free(line);
}


/******************************************Error prints********************************************/

void tooManyArgs(char* command, int howMany)
{
	if(howMany > 0)
	{
		printf("Error: too many arguments, command %s expecting %d arguments.\n", command, howMany);
	}
	else
	{
		printf("Error: too many arguments, command %s expecting no arguments.\n", command);
	}
}

void tooFewArgs(char* command, int howMany)
{
	printf("Error: not enough command, %s expecting %d arguments.\n", command, howMany);
}

void erroneusBoard()
{
	printf("Error: the current board is erroneous.\n");
}

void unsolvableBoard()
{
	printf("Error: the board is unsolvable.\n");
}

void legalMode(int num, char* currCommand, char* command1, char* command2)
{
	if(num == 1)
	{
		printf("Error: command %s is invalid in this mode.\nOnly available in %s mode.\n", currCommand, command1);
	}
	else
	{
		printf("Error: command %s is invalid in this mode.\nOnly available in %s and %s modes.\n", currCommand, command1, command2);
	}
}

void faildMalloc()
{
	printf("Error: malloc has failed.\n");
	exit(0);
}
