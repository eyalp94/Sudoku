#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Cell.h"
#include "files.h"
#include "linkedList.h"
#include "Printer.h"
#include "Solver.h"

/*parse the user's command*/
int interpreter(linkedList* reun)
{
	static int n = 0;
	static int m = 0;
	char command[1024];
	char* token, *tokenToCheck;
	char* xyzc[3];
	static int mode = 0;			/*0 - Init, 1 - Edit, 2 - Solve*/
	static int mark_errors = 1;
	static int mark_in_edit = 0;
	static Cell** board = NULL;
	static Cell** clone = NULL;
	static Cell** solution = NULL;
	Cell** cloneSolutions;
	int cnt = 0, xyz[3] = {0}, flag2 = 0, flag3 = 0, nm[2], setResult, bef = 0, k, isnum;
	float threshold;
	void* flag4;
	scanf(" ");
	flag4 = fgets(command, 1024, stdin);
	if(flag4 == NULL)
		prinExit(n,m,board,solution, clone, reun);
	while(command[0] == '\n' || (command[0] == '\r' && command[1] == '\n'))
		fgets(command, 1024, stdin);
	if(strlen(command) > 256)
	{
		printf("Error: command over 256 characters.\n");
		return 0;
	}
	token = strtok(command, " \t\r\n");
	/*************************************************************************************solve*/
	if(strcmp(token, "solve") == 0)
	{
		token = strtok(NULL, " \t\r\n");
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(tokenToCheck != NULL)
		{
			tooManyArgs("solve", 1);
			return 0;
		}
		if(token != NULL)
		{
			if(board != NULL)
			{
				freeBoard(n, m, board);
				board = NULL;
			}
			if(clone != NULL)
			{
				freeBoard(n, m, clone);
				clone = NULL;
			}
			if(solution != NULL)
			{
				freeBoard(n, m, solution);
				solution = NULL;
			}
			board = readFile(token, nm, 2);
			if(board == NULL)
			{
				return 0;
			}
			clone = createBoard(nm[0], nm[1]);
			solution = createBoard(nm[0], nm[1]);
			cloneCopy(clone, board, nm[0], nm[1]);
			deleteAll(reun);
			reun->curr = reun->first;
			n = nm[0];
			m = nm[1];
			mode = 2;
			mark_in_edit = 0;
			printBoard(n, m, board, mark_errors + mark_in_edit);
		}
		else
		{
			printf("Error: must enter path.\n");
		}
	}
	/**************************************************************************************edit*/
	else if(strcmp(token, "edit") == 0)
	{
		token = strtok(NULL, " \t\r\n");
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(tokenToCheck != NULL)
		{
			tooManyArgs("edit", 1);
			return 0;
		}
		if(board != NULL)
		{
			freeBoard(n, m, board);
			board = NULL;
		}
		if(clone != NULL)
		{
			freeBoard(n, m, clone);
			clone = NULL;
		}
		if(solution != NULL)
		{
			freeBoard(n, m, solution);
			solution = NULL;
		}
		if(token != NULL)
		{
			board = readFile(token, nm, 1);
			if(board == NULL)
			{
				return 0;
			}
			clone = createBoard(nm[0], nm[1]);
			solution = createBoard(nm[0], nm[1]);
			cloneCopy(clone, board, nm[0], nm[1]);
			deleteAll(reun);
			reun->curr = reun->first;
			n = nm[0];
			m = nm[1];
		}
		else
		{
			board = createBoard(3, 3);
			clone = createBoard(3, 3);
			solution = createBoard(3, 3);
			n = 3;
			m = 3;
			nm[0] = 3;
			nm[1] = 3;
		}
		mode = 1;
		mark_in_edit = 1;
		printBoard(n, m, board, mark_errors + mark_in_edit);
	}
	/*******************************************************************************mark errors*/
	else if(strcmp(token, "mark_errors") == 0)
	{
		token = strtok(NULL, " \t\r\n");
		tokenToCheck = strtok(NULL, " \t\r\n");
		if (mode != 2)
		{
			legalMode(1, "mark_errors", "solve", NULL);
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("mark errors", 1);
			return 0;
		}
		if(token != NULL && (isNum(token) == 1))
		{
			if((strtol(token, NULL, 10) == 1 || strtol(token, NULL, 10) == 0))
			{
				mark_errors = strtol(token, NULL, 10);
				markErrors(n, m, board);
			}
			else
			{
				printf("Error: invalid value for mark errors, should be 0 or 1.\n");
			}
		}
		else
		{
			printf("Error: invalid value for mark errors, should be 0 or 1.\n");
		}
	}
	/*******************************************************************************print board*/
	else if(strcmp(token, "print_board") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if (mode == 0)
		{
			legalMode(2, "print_board", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("print board", 0);
			return 0;
		}
		printBoard(n, m, board, mark_errors + mark_in_edit);
	}
	/***************************************************************************************set*/
	else if(strcmp(token, "set") == 0)
	{
		if(mode == 0)
		{
			legalMode(2, "set", "edit", "solve");
			return 0;
		}
		cnt = 0;
		token = strtok(NULL, " \t\r\n");
		while (token != NULL)
		{
			if(cnt >= 3)
			{
				tooManyArgs("set", 3);
				return 0;
			}
			xyzc[cnt] = token;
			cnt++;
			token = strtok(NULL, " \t\r\n");
		}
		if (cnt < 3)
		{
			tooFewArgs("set", 3);
			return 0;
		}
		for(k = 0; k < 3; k++)
		{
			isnum = isNum(xyzc[k]);
			if(isnum == 0)
			{
				printf("Error: argument number %d should be a positive integer.\n", k + 1);
				return 0;
			}
			else if(isnum == 2)
			{
				printf("Error: please enter numbers only (without a dot).\n");
				return 0;
			}
			xyz[k] = strtol(xyzc[k], NULL, 10);
			if(k == 2 && (xyz[k] < 0 || xyz[k] > n*m))
			{
				printf("Error: argument number %d should be in range 0 - %d.\n", k + 1, n*m);
				return 0;
			}
			if(k != 2 && (xyz[k] <= 0 || xyz[k] > n*m))
			{
				printf("Error: argument number %d should be in range 1 - %d.\n", k + 1, n*m);
				return 0;
			}
		}
		xyz[0]--;
		xyz[1]--;
		bef = board[xyz[1]][xyz[0]].value;
		deleteFromHere(reun, reun->curr->next);
		setResult = set(xyz, board ,clone, mode);
		if(setResult == 1)
		{
			addNode(reun, xyz[1],xyz[0],bef,xyz[2], 0);
			printBoard(n, m, board, mark_errors + mark_in_edit);
		}
		else if(setResult == 2)
		{
			printBoard(n, m, board, mark_errors + mark_in_edit);
		}
	}
	/**********************************************************************************validate*/
	else if(strcmp(token, "validate") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "validate", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("validate", 0);
			return 0;
		}
		if(!checkBoard(n, m, board))
		{
			erroneusBoard();
			return 0;
		}
		flag2 = ilpSolver(n, m, board, solution);
		if (flag2 == 2)
		{
			printf("Validation passed: board is solvable.\n");
		}
		else if(flag2 == 1)
		{
			printf("Validation failed: board is unsolvable.\n");
		}
		else
		{
			printf("Error: unable to solve the ILP due to an error in the build process.\n");
		}
		return 0;
	}
	/*************************************************************************************guess*/
	else if (strcmp(token, "guess") == 0)
	{
		token = strtok(NULL, " \t\r\n");
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode != 2)
		{
			legalMode(1, "guess", "solve", NULL);
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("guess", 1);
			return 0;
		}
		if(token == NULL)
		{
			printf("Error: must enter threshold.\n");
			return 0;
		}
		if(!sscanf(token, "%f", &threshold))
		{
			printf("Error: threshold must be of type float.\n");
			return 0;
		}
		if(threshold < 0 || threshold > 1)
		{
			printf("Error: threshold must be between 0 and 1.\n");
			return 0;
		}
		deleteFromHere(reun, reun->curr->next);
		guess(n, m, threshold, board, clone, reun);
		printBoard(n, m, board, mark_errors + mark_in_edit);
	}
	/**********************************************************************************generate*/
	else if (strcmp(token, "generate") == 0)
	{
		cnt = 0;
		token = strtok(NULL, " \t\r\n");
		if(mode != 1)
		{
			legalMode(1, "generate", "edit", NULL);
			return 0;
		}
		while (token != NULL)
		{
			if(cnt >= 2)
			{
				tooManyArgs("generate", 2);
				return 0;
			}
			xyzc[cnt] = token;
			cnt++;
			token = strtok(NULL, " \t\r\n");
		}
		if (cnt < 2)
		{
			tooFewArgs("generate", 2);
			return 0;
		}
		for(k = 0; k < 2; k++)
		{
			isnum = isNum(xyzc[k]);
			if(isnum == 0)
			{
				printf("Error: argument number %d should be a positive integer.\n", k + 1);
				return 0;
			}
			else if(isnum == 2)
			{
				printf("Error: please enter numbers only (without a dot).\n");
				return 0;
			}
			xyz[k] = strtol(xyzc[k], NULL, 10);
			if(xyz[k] < 0 || xyz[k] > n*m*n*m)
			{
				printf("Error: argument number %d should be in range 0 - %d.\n", k + 1, n*m*n*m);
				return 0;
			}
		}
		deleteFromHere(reun, reun->curr->next);
		generate(n, m, xyz[0], xyz[1], board, clone, solution, reun);
		printBoard(n, m, board, mark_errors + mark_in_edit);
	}
	/**************************************************************************************undo*/
	else if (strcmp(token, "undo") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "undo", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("undo", 0);
			return 0;
		}
		else
		{
			if(reun->curr == reun->first)
			{
				printf("Error: no move to undo.\n");
			}
			else
			{
				undo(reun, board, clone, 0);
				printBoard(n, m, board, mark_errors + mark_in_edit);
			}
		}
		return 0;
	}
	/**************************************************************************************redo*/
	else if (strcmp(token, "redo") == 0)
	{

		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "redo", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("redo", 0);
			return 0;
		}
		else
		{
			if(reun->curr->next == reun->last)
			{
				printf("Error: no move to redo.\n");
			}
			else
			{
				redo(reun, board, clone);
				printBoard(n, m, board, mark_errors + mark_in_edit);
			}
		}
		return 0;
	}
	/**************************************************************************************save*/
	else if (strcmp(token, "save") == 0)
	{
		token = strtok(NULL, " \t\r\n");
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "save", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("save", 1);
			return 0;
		}
		if (token == NULL)
		{
			printf("Error: must enter path.\n");
			return 0;
		}
		if(mode == 1)
		{
			if(!checkBoard(n, m, board))
			{
				printf("Error: erroneous boards can't be saved.\n");
				return 0;
			}
			else if(!validate(n, m, board, solution))
			{
				printf("Error: unsolvable boards can't be saved.\n");
				return 0;
			}
			else
			{
				turnToFixed(n, m, clone);
			}
		}
		if(saveBoard(n, m, clone, token))
		{
			printf("Saved successfully.\n");
		}
		return 0;
	}
	/**************************************************************************************hint*/
	else if (strcmp(token, "hint") == 0)
	{
		cnt = 0;
		token = strtok(NULL, " \t\r\n");
		if(mode != 2)
		{
			legalMode(1, "hint", "solve", NULL);
			return 0;
		}
		while(token != NULL)
		{
			if(cnt >= 2)
			{
				tooManyArgs("hint", 2);
				return 0;
			}
			xyzc[cnt] = token;
			cnt++;
			token = strtok(NULL, " \t\r\n");
		}
		if(cnt < 2)
		{
			tooFewArgs("hint", 2);
			return 0;
		}
		for(k = 0; k < 2; k++)
		{
			isnum = isNum(xyzc[k]);
			if(isnum == 0)
			{
				printf("Error: argument number %d should be a positive integer.\n", k + 1);
				return 0;
			}
			else if(isnum == 2)
			{
				printf("Error: please enter numbers only (without a dot).\n");
				return 0;
			}
			xyz[k] = strtol(xyzc[k], NULL, 10);
			xyz[k]--;
			if(xyz[k] < 0 || xyz[k] >= n*m)
			{
				printf("Error: argument number %d should be in range 1 - %d.\n", k + 1, n*m);
				return 0;
			}
		}
		if(!checkBoard(n, m, board))
		{
			erroneusBoard();
			return 0;
		}
		if(board[xyz[1]][xyz[0]].fixed == 1)
		{
			printf("Error: cell is fixed.\n");
			return 0;
		}
		if(board[xyz[1]][xyz[0]].value)
		{
			printf("Error: cell already contains a value.\n");
			return 0;
		}
		hint(n, m, xyz, board, solution);
		return 0;
	}
	/********************************************************************************guess hint*/
	else if (strcmp(token, "guess_hint") == 0)
	{
		cnt = 0;
		token = strtok(NULL, " \t\r\n");
		if(mode != 2)
		{
			legalMode(1, "guess_hint", "solve", NULL);
			return 0;
		}
		while(token != NULL)
		{
			if(cnt >= 2)
			{
				tooManyArgs("guess hint", 2);
				return 0;
			}
			xyzc[cnt] = token;
			cnt++;
			token = strtok(NULL, " \t\r\n");
		}
		if(cnt < 2)
		{
			tooFewArgs("guess hint", 2);
			return 0;
		}
		for(k = 0; k < 2; k++)
		{
			isnum = isNum(xyzc[k]);
			if(isnum == 0)
			{
				printf("Error: argument number %d should be a positive integer.\n", k + 1);
				return 0;
			}
			else if(isnum == 2)
			{
				printf("Error: please enter numbers only (without a dot).\n");
				return 0;
			}
			xyz[k] = strtol(xyzc[k], NULL, 10);
			xyz[k]--;
			if(xyz[k] < 0 || xyz[k] >= n*m)
			{
				printf("Error: argument number %d should be in range 1 - %d.\n", k + 1, n*m);
				return 0;
			}
		}
		guessHint(n, m, board, clone, reun, xyz[1], xyz[0]);
		return 0;
	}
	/*****************************************************************************num solutions*/
	else if(strcmp(token, "num_solutions") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "num_solutions", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("num solution", 0);
			return 0;
		}
		if(!checkBoard(n, m, board))
		{
			erroneusBoard();
			return 0;
		}
		else
		{
			if(emptyCells(n, m, board) == 0)
			{
				printf("The number of solutions is: 1.\n");
			}
			else
			{
				cloneSolutions = createBoard(n, m);
				cloneCopy(cloneSolutions, board, n, m);
				turnToFixed1(n, m, cloneSolutions);
				printf("The number of solutions is: %d.\n",exhaustiveBacktracking(n, m, cloneSolutions));
				freeBoard(n,m,cloneSolutions);
			}
		}
		return 0;
	}
	/**********************************************************************************autofill*/
	else if (strcmp(token, "autofill") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode != 2)
		{
			legalMode(1, "autofill", "solve", NULL);
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("autofill", 0);
			return 0;
		}
		if(!checkBoard(n, m, board))
		{
			erroneusBoard();
			return 0;
		}
		deleteFromHere(reun, reun->curr->next);
		autoFill(n, m, board, clone, reun);
		reun->curr = reun->last->prev;
		printBoard(n, m, board, mark_errors + mark_in_edit);
	}
	/*************************************************************************************reset*/
	else if (strcmp(token, "reset") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(mode == 0)
		{
			legalMode(2, "reset", "edit", "solve");
			return 0;
		}
		if(tokenToCheck != NULL)
		{
			tooManyArgs("reset", 0);
			return 0;
		}
		while(reun->curr != reun->first)
		{
			undo(reun, board, clone, 1);
		}
		printBoard(n, m, board, mark_errors + mark_in_edit);
		return 0;
	}
	/**************************************************************************************exit*/
	else if (strcmp(token, "exit") == 0)
	{
		tokenToCheck = strtok(NULL, " \t\r\n");
		if(tokenToCheck != NULL)
		{
			tooManyArgs("exit", 0);
			return 0;
		}
		prinExit(n, m, board, solution, clone, reun);
	}
	/*******************************************************************************************/
	else
	{
		printf("Error: invalid command.\n");
		return 0;
	}
	if(mode == 2 && (!emptyCells(n, m, board)))
	{
		if(checkBoard(n, m, board))
		{
			mode = 0;
			mark_in_edit = 0;
			printf("Puzzle solved successfully!\nReturned to Init mode.\n");	/*return to mode Init*/
		}
		else
		{
			printf("The solution is erroneous.\n");
			return 0;
		}
	}
	return flag3;
}
