PHONY = all clean
CCFLAGS=-Wall -Wno-unused-function -pedantic -std=c99 -g -DYYDEBUG=1
YLFLAGS= -ll -ly
MKDIR_BIN = bin
SRC_FOLDER = src

all: bin parser

PARSER_OBJ = compiler.o lex.yy.o y.tab.o

PROGRAM_OBJ = program temp.c

OBJ = lex.yy.c y.tab.c y.tab.h

GCC_CXXFLAGS = -DMESSAGE='"Compiled with GCC"'
CLANG_CXXFLAGS = -DMESSAGE='"Compiled with Clang"' -Wno-gnu-zero-variadic-macro-arguments
UNKNOWN_CXXFLAGS = -DMESSAGE='"Compiled with an unknown compiler"'

ifeq ($(CC),g++)
  CCFLAGS += $(GCC_CXXFLAGS)
else ifeq ($(CC),clang)
  CCFLAGS += $(CLANG_CXXFLAGS)
else
  CCFLAGS += $(UNKNOWN_CXXFLAGS)
endif

parser:
	lex $(SRC_FOLDER)/lexer.l
	yacc -d $(SRC_FOLDER)/parser.y
	$(CC) $(CCFLAGS) $(SRC_FOLDER)/tree_translation.c $(SRC_FOLDER)/tree.c lex.yy.c y.tab.c $(SRC_FOLDER)/compiler.c $(SRC_FOLDER)/var_finder.c $(YLFLAGS) -o $(MKDIR_BIN)/compiler
	rm -rf $(OBJ)

bin: $(shell mkdir -p $(MKDIR_BIN))

clean: 
	rm -rf $(PARSER_OBJ) $(PROGRAM_OBJ) $(OBJ) $(MKDIR_BIN)/compiler
