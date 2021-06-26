%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tree.h"


// Extern prototypes

extern int yylex();
extern void yyerror(node_list ** program, char *s);

extern FILE * out;
extern void * malloc();

// Auxiliar macros

#define P(...) fprintf(out, ##__VA_ARGS__);

#define ERROR(...) fprintf(stderr, "\x1b[31mERROR: ");\
    fprintf(stderr, ##__VA_ARGS__);\
    fprintf(stderr, "\x1b[0m\n");\
    exit(-1);

// Global variables

int yydebug=1;
int recursion = 0;

%}

%union {
    char string[1024];
    double array[1024];
    double number;
    char boolean;
    node_t * node;
    node_list * list;
}

%token START
%token MEAN MEDIAN MODE STDEV RANGE QTR1 QTR3 INTER_QTR GCD MCM
%token COMB PERM FACT
%token OF

%token ASSIGN
%token <string> IF WHILE DO END ELSE
%token EOL FIN
%token <number> TEXT_TYPE NUMBER_TYPE BOOLEAN_TYPE LIST_TYPE
%token WRITE

%token <string> SYMBOL_NAME
%token <string> BIN_OP UNI_OP
%token <string> NUMBER TEXT BOOLEAN LIST
%token <string> FUNCTION_CALL

%type <number> type
%type <node> full_declare declare assign value expression 
%type <node> instruction write function_call if if_end while
%type <list> program block

%left BIN_OP
%nonassoc UNI_OP

%parse-param {node_list ** program}

%%
program: instruction program { $$ = (*program = (node_list *)add_element_to_list($2, $1)); }
    | EOL program { $$ = $2; }
    | FIN { $$ = (*program = (node_list *)add_instruction_list_node(NULL)); };

instruction: full_declare { $$ = add_instruction_node($1); }
    | assign { $$ = add_instruction_node($1); }
    | write { $$ = add_instruction_node($1); }
    | function_call  { $$ = add_instruction_node($1); }
    | if { $$ = add_instruction_node($1); }
    | while { $$ = add_instruction_node($1); };

block: instruction block { $$ = (node_list *)add_element_to_list($2, $1); }
    | EOL block { $$ = $2; }
    | instruction { $$ = (node_list *)add_instruction_list_node($1); }
    | EOL { $$ = (node_list *)add_instruction_list_node(NULL); };

if: IF expression DO block if_end { $$ = add_if_node($2, add_block_node($4), $5); };

while: WHILE expression DO block END { $$ = add_while_node($2, add_block_node($4)); };

if_end: END { $$ = NULL; }
    | ELSE block END { $$ = add_block_node($2); };
    
full_declare: declare               { $$ = $1; }
    | declare ASSIGN value          { $$ = add_value_variable($1, $3); };

declare: type SYMBOL_NAME       { $$ = declare_variable_node($2, $1); };
type: NUMBER_TYPE | TEXT_TYPE | BOOLEAN_TYPE | LIST_TYPE;

assign: SYMBOL_NAME ASSIGN value { $$ = assign_variable_node($1, $3); };
value: expression   { $$ = $1; }
    | SYMBOL_NAME   { $$ = add_variable_reference($1); }
    | TEXT          { $$ = add_text_node($1); }
    | LIST          { $$ = add_list_node($1); };

function_call: FUNCTION_CALL LIST           { $$ = add_function_call($1, add_list_node($2)); }
    | FUNCTION_CALL SYMBOL_NAME             { $$ = add_function_call($1, add_variable_reference($2)); };

write: WRITE expression                     { $$ = add_print_node($2); }
    | WRITE SYMBOL_NAME                     { $$ = add_print_node(add_variable_reference($2)); }
    | WRITE TEXT                            { $$ = add_print_node(add_text_node($2)); }
    | WRITE LIST                            { $$ = add_print_node(add_list_node($2)); };

expression: '(' expression ')'              { $$ = add_expression_node(add_operation_node("("), $2, add_operation_node(")")); }
    | UNI_OP expression                     { $$ = add_expression_node(add_operation_node($1), $2, NULL); }
    | expression BIN_OP expression          { $$ = add_expression_node($1, add_operation_node($2), $3); }
    | function_call                         { $$ = add_expression_node($1, NULL, NULL); }
    | NUMBER                                { $$ = add_expression_node(add_number_node($1), NULL, NULL); }
    | '$' SYMBOL_NAME                       { $$ = add_expression_node(add_variable_reference($2), NULL, NULL); };
%%
