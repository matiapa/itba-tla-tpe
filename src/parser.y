%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/tree.h"
#include "include/free_tree.h"


// Extern prototypes

extern int yylex();
void yyerror(node_list ** program, char *s);
void warning(char * s);

extern FILE * out;
extern void * malloc();

#define WARNING(...) fprintf(stderr, "\033[38;2;255;165;0mWARNING: ");\
    fprintf(stderr, ##__VA_ARGS__);\
    fprintf(stderr, "\x1b[0m\n");\
    ;

// Global variables

int yydebug = 1;
int recursion = 0;
int main_init = FALSE;
int reduced = 0;

%}

%union {
    char string[1024];
    double array[1024];
    double number;
    char boolean;
    node_t * node;
    node_list * list;
}

%token START EOL FIN
%token COMB PERM 
%token MEAN MEDIAN MODE STDEV RANGE QTR1 QTR3 INTER_QTR IN
%token ASSIGN WRITE READ

%token <string> IF WHILE DO END ELSE
%token <string> SYMBOL_NAME

%token <string> BIN_OP UNI_OP UMINUS MEASURE_OF POWER FACT
%token <string> NUMBER TEXT BOOLEAN LIST

%token <number> TEXT_TYPE NUMBER_TYPE BOOLEAN_TYPE LIST_TYPE
%token <number> NATURAL

%type <number> type
%type <node> full_declare declare assign value expression list_operation list_value
%type <node> instruction write read if if_end while
%type <list> program block

%nonassoc IN
%left BIN_OP '-'
%left POWER
%left UNI_OP
%left FACT

%parse-param {node_list ** program}

%%
program: instruction program { $$ = (*program = (node_list *)add_element_to_list($2, $1)); }
    | EOL program { $$ = $2; }
    | FIN { $$ = (*program = (node_list *)add_instruction_list_node(NULL)); };

instruction: full_declare { $$ = add_instruction_node($1); }
    | assign    { $$ = add_instruction_node($1); }
    | write     { if (main_init == FALSE) {
                    $$ = free_write($1); 
                    warning("write");
                  } else $$ = add_instruction_node($1); }
    | read      { if (main_init == FALSE) {
                    $$ = free_read($1);
                    warning("read");
                  } else $$ = add_instruction_node($1); }
    | if        { if (main_init == FALSE) {
                    $$ = free_if_node($1);
                    warning("if");
                  } else $$ = add_instruction_node($1); }
    | while     { if (main_init == FALSE) {
                    $$ = free_while_node($1);
                  } else $$ = add_instruction_node($1); }
    | START     { main_init=TRUE; $$=NULL; };

block: instruction block { $$ = (node_list *)add_element_to_list($2, $1); }
    | EOL block { $$ = $2; }
    | instruction { $$ = (node_list *)add_instruction_list_node($1); }
    | EOL { $$ = (node_list *)add_instruction_list_node(NULL); };

if: IF expression DO block if_end { $$ = add_if_node($2, add_block_node($4), $5); };

while: WHILE expression DO block END { $$ = add_while_node($2, add_block_node($4)); };

if_end: END { $$ = NULL; }
    | ELSE block END { $$ = add_block_node($2); };
    
full_declare: declare               { $$ = $1; }
    | declare ASSIGN value          { $$ = add_value_variable($1, $3); }
    | LIST_TYPE SYMBOL_NAME ASSIGN value { $$ = add_value_variable(declare_variable_node($2, $1), $4); };

declare: type SYMBOL_NAME           { $$ = declare_variable_node($2, $1); };
type: NUMBER_TYPE | TEXT_TYPE | BOOLEAN_TYPE ;

assign: SYMBOL_NAME ASSIGN value { $$ = assign_variable_node($1, $3); };
value: expression   { $$ = $1; }
    | SYMBOL_NAME   { $$ = add_variable_reference($1); }
    | TEXT          { $$ = add_text_node($1); }
    | LIST          { $$ = add_array_node($1); };

write: WRITE expression                     { $$ = add_print_node($2); }
    | WRITE SYMBOL_NAME                     { $$ = add_print_node(add_variable_reference($2)); }
    | WRITE TEXT                            { $$ = add_print_node(add_text_node($2)); }
    | WRITE LIST                            { $$ = add_print_node(add_array_node($2)); };

read: READ SYMBOL_NAME                      { $$ = add_read_node(add_variable_reference($2)); };

expression: '(' expression ')'              { $$ = add_expression_node(add_operation_node("("), $2, add_operation_node(")")); }
    | UNI_OP expression                     { $$ = add_expression_node(add_operation_node($1), $2, NULL); }
    | '-' expression           %prec UNI_OP { $$ = add_expression_node(add_operation_node("-"), $2, NULL); }
    | expression BIN_OP expression          { $$ = add_expression_node($1, add_operation_node($2), $3); }
    | expression '-' expression             { $$ = add_expression_node($1, add_operation_node("-"), $3); }
    | expression POWER expression           { $$ = add_expression_node($1, add_operation_node("^"), $3); }
    | expression FACT                       { $$ = add_expression_node($1, add_operation_node($2), NULL); }
    | list_operation                        { $$ = add_expression_node($1, NULL, NULL); }
    | NUMBER                                { $$ = add_expression_node(add_number_node($1), NULL, NULL); }
    | '$' SYMBOL_NAME                       { $$ = add_expression_node(add_variable_reference($2), NULL, NULL); };

list_operation: MEASURE_OF list_value       { $$ = add_list_operation($1, $2, add_expression_node(add_number_node("0"), NULL, NULL)); }
    | list_value '(' expression ')'         { $$ = add_list_operation("elem_at", $1, $3); }
    | expression IN list_value              { $$ = add_list_operation("contains", $3, $1); };

list_value: SYMBOL_NAME                     { $$ = add_variable_reference($1); }
    | LIST                                  { $$ = add_array_node($1); }

%%


void warning(char * s) {
    extern int yylineno;
    WARNING("%s at line %d, will not be considered.", s, yylineno);
}
