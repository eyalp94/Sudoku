/*
 * files.c
 *
 *  Created on: 30 αιεμ 2019
 *      Author: shake
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cell.h"
#include "Printer.h"

/*returns the length of the longest line in the file*/
int maxLineLength(char* fileName)
{
	FILE* fp;
	int ch;
	int maxC = 0, cnt = 0;
	fp = fopen(fileName, "r");
	if(fp == NULL)
	{
		printf("Error occurred during opening the file.\n");
		return -1;
	}
	while(1)
	{
		ch = getc(fp);
		if(ch == EOF)
			break;
		if(ch == '\n')
		{
			if(maxC < cnt)
			{
				maxC = cnt;
			}
			cnt = 0;
			continue;
		}
		cnt++;
	}
	if (fclose(fp) == EOF)
	{
		printf("Error occurred during closing the file.\n");
		exit(0);
	}
	return maxC;
}

/*checks whether the string represent a number*/
int isNum(char* str)
{
	int i;
	unsigned int len = strlen(str);
	for(i = 0; i < (int)len; i++)
	{
		if (i == ((int)len - 1))
		{
			if (str[i] == '.')
			{
				return 2;
			}
		}
		if((str[i] < '0' || str[i] > '9'))
		{
			return 0;
		}
	}
	return 1;
}

/*fills an empty board according to the file*/
int boardFromFile(FILE* fp, int m, int n, int ml,  Cell** board, int mode)
{
	int i = 0, j = 0, cnt = 0, temp;
	char* line, *token;
	line = (char*)malloc(ml + 1);
	if(line == NULL)
	{
		faildMalloc();
	}
	while(fgets(line, ml*20, fp) != NULL)
	{
		token = strtok(line, " \t\r\n");
		while(token != NULL)
		{
			temp = isNum(token);
			if(temp != 0)
			{
				if(temp > n*m)
				{
					printf("Error: cell's value can't be greater then %d!\n", n*m);
					prinExitFile(fp, board, n, m, line);
					return 0;
				}
				board[i][j].value = strtol(token, NULL, 10);
				if (temp == 2)
				{
					if(board[i][j].value == 0)
					{
						printf("Error: fixed cells must contain a number!\n");
						prinExitFile(fp, board, n, m, line);
						return 0;
					}
					if(mode == 2)
					{
						board[i][j].fixed = 1;
					}
				}
				cnt++;
				token = strtok(NULL, " \t\r\n");
				j++;
			}
			else
			{
				printf("Error: all cells must contain a number!\n");
				prinExitFile(fp, board, n, m, line);
				return 0;
			}
		}
		if(j == n*m)
		{
			i++;
			j = 0;
		}
		if(i == n*m && j == n*m)
		{
			break;
		}
	}
	if(cnt < n*m*n*m)
	{
		printf("Error: all cells must contain a number!\n");
		prinExitFile(fp, board, n, m, line);
		return 0;
	}
	if(cnt > n*m*n*m)
	{
		printf("Error: too many values are supplied in the file!\n");
		prinExitFile(fp, board, n, m, line);
		return 0;
	}
	free(line);
	return 1;
}

/*reads the file and returns the board that was in it*/
Cell** readFile(char* fileName, int* nm, int mode)
{
	Cell** board, **clone;
	char* line, *token;
	int ml, n, m, flag;
	FILE* fp;
	fp = fopen(fileName, "r");
	if(fileName == NULL)
	{
		printf("Error: Invalid file name!\n");
		return NULL;
	}
	if(fp == NULL){
		printf("Error: Invalid path or file not found!\n");
		return NULL;
	}
	ml = maxLineLength(fileName);
	if(ml == -1)
	{
		return NULL;
	}
	ml++;
	line = (char*)malloc(ml);
	if(line == NULL)
	{
		faildMalloc();
	}
	fgets(line, ml, fp);
	token = strtok(line, " \t\r\n");
	if(isNum(token))
	{
		m = strtol(token, NULL, 10);
	}
	else
	{
		printf("Error: size of m must be a number.\n");
		free(line);
		return NULL;
	}
	token = strtok(NULL, " \t\r\n");
	if(isNum(token))
	{
		n = strtol(token, NULL, 10);
	}
	else
	{
		printf("Error: size of n must be a number.\n");
		free(line);
		return NULL;
	}
	if(n*m > 99)
	{
		printf("Error: n*m is at most 99.\n");
		free(line);
		return NULL;
	}
	board = createBoard(n,m);
	flag = boardFromFile(fp, m, n, ml, board, mode);
	if(!flag)
	{
		free(line);
		freeBoard(n, m, board);
		return NULL;
	}
	clone = createBoard(n, m);
	cloneCopy(clone, board, n, m);
	eraseNotFix(n, m, clone);
	if(!checkBoard(n, m, clone))
	{
		printf("Error: fixed cell can't be erroneous.\n");
		free(line);
		freeBoard(n, m, board);
		freeBoard(n, m, clone);
		return NULL;
	}
	free(line);
	freeBoard(n, m, clone);
	if(fclose(fp) == EOF)
	{
		printf("Error occurred during closing the file.\n");
		exit(0);
	}
	nm[0] = n;
	nm[1] = m;
	return board;

}

/*saves the current board to a file*/
int saveBoard(int n, int m, Cell** board, char* path)
{
	FILE* fp = fopen(path, "w");
	int i, j = 0;
	if(path == NULL)
	{
		printf("Error: Invalid input, must enter path!\n");
		return 0;
	}
	if(fp == NULL)
	{
		printf("Error: Invalid path!\n");
		return 0;
	}
	putc(m + '0', fp);
	putc(' ', fp);
	putc(n + '0', fp);
	putc('\n', fp);
	for(i = 0; i < n*m; i++)
	{
		for(j = 0; j < n*m; j++)
		{
			putc(board[i][j].value + '0', fp);
			if(board[i][j].fixed)
			{
				putc('.', fp);
			}
			if(j != n*m - 1)
			{
				putc(' ', fp);
			}
		}
		if (i != n*m - 1)
		{
			putc('\n', fp);
		}
	}
	if(fclose(fp) == EOF)
	{
		printf("Error occurred during closing the file.\n");
		exit(0);
	}
	return 1;
}





