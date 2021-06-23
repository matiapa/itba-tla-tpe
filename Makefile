PHONY = all clean
CCFLAGS=-Wall -pedantic -std=c99
YLFLAGS= -ll
MKDIR_BIN = bin

all: bin parser

PARSER_OBJ = compiler.o lex.yy.o y.tab.o

OBJ = lex.yy.c y.tab.c y.tab.h

parser:
	lex lexer.l
	yacc -d parser.y
	$(CC) $(YLFLAGS) $(CCFLAGS) lex.yy.c y.tab.c compiler.c -o $(MKDIR_BIN)/compiler
	rm -rf $(OBJ)

bin: $(shell mkdir -p $(MKDIR_BIN))

clean: 
	rm -rf $(PARSER_OBJ) $(OBJ) $(MKDIR_BIN)/compiler
