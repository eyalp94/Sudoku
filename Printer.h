/*
 * Printer.h
 *
 *  Created on: 29 במאי 2019
 *      Author: shake
 */

#ifndef PRINTER_H_
#define PRINTER_H_
#include "Parser.h"
#include "Game.h"
#include "Cell.h"
#include "stack.h"
#include "linkedList.h"

void printBoard(int n, int m, Cell** board, int mark_errors);
void prinExit(int n, int m, Cell** board, Cell** solution, Cell** clone, linkedList* reun);
void prinExitFile(FILE* fp, Cell** board, int n, int m, char* line);
void tooManyArgs(char* command, int howMany);
void tooFewArgs(char* command, int howMany);
void erroneusBoard();
void unsolvableBoard();
void legalMode(int num, char* currCommand, char* command1, char* command2);
void faildMalloc();

#endif /* PRINTER_H_ */
