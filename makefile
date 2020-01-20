CC = gcc
OBJS = main.o Solver.o Game.o Parser.o linkedList.o Cell.o files.o stack.o Printer.o
EXEC = sudoku-console
COMP_FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Game.h SPBufferset.h linkedList.h Solver.h files.h stack.h Parser.h Cell.h stack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Solver.o: Solver.c Solver.h Game.h stack.h Parser.h Cell.h Printer.h linkedList.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h Solver.h linkedList.h Printer.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h Game.h Solver.h linkedList.h Printer.h files.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
linkedList.o: linkedList.c linkedList.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Printer.o: Printer.c Printer.h Game.h Parser.h Solver.h Cell.h stack.h linkedList.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
files.o: files.c files.h Printer.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
stack.o: stack.c stack.h Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Cell.o: Cell.c Cell.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)
