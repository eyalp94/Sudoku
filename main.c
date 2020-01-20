/*
 * main.c
 *
 *  Created on: 28 במאי 2019
 *      Author: shake
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "Parser.h"
#include "Game.h"
#include "Solver.h"
#include "Printer.h"
#include "Cell.h"
#include "linkedList.h"
#include "files.h"
#include "SPBufferset.h"

int main()
{
	linkedList* reun = createList();
	SP_BUFF_SET();
	setbuf(stdout, NULL);
	game(reun);
	return 0;
}
