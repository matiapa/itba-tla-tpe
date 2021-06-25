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

// Local prototypes

int lookup_variable(char * name);
void declare_variable(int type, char * name);
void assign_variable(char * name, char * value, int attr_type);
void write_symbol(char * name);
void write_expression(char * exp1, char * op, char * exp2);

char * type_desc(int type);
void assert_type(int type, char * var);

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
%token <number> TEXT_TYPE NUMBER_TYPE BOOLEAN_TYPE ARRAY_TYPE
%token WRITE

%token <string> SYMBOL_NAME
%token <string> BIN_OP UNI_OP
%token <string> NUMBER TEXT BOOLEAN ARRAY

%type <number> type
%type <node> declare full_declare value assign instruction write expression write_expression
%type <list> program 

%left BIN_OP
%nonassoc UNI_OP

%parse-param {node_list ** program}

%%
program: instruction program { $$ = (*program = (node_list *)add_element_to_list($2, $1)); }
    | EOL program { $$ = $2; }
    | FIN { $$ = (*program = (node_list *)add_instruction_list_node(NULL)); };

instruction: full_declare { $$ = add_instruction_node($1); }
    | assign { $$ = add_instruction_node($1); }
    | write { $$ = add_instruction_node($1); };

full_declare: declare '=' value { $$ = add_value_variable($1, $3);}
    | declare { $$ = $1; };

declare: type SYMBOL_NAME { $$ = declare_variable_node($2, $1); };

type: NUMBER_TYPE | TEXT_TYPE | BOOLEAN_TYPE | ARRAY_TYPE;

assign: SYMBOL_NAME '=' value { $$ = assign_variable_node($1, $3); };

value: expression { $$ = $1; } 
    | TEXT { $$ = add_text_node($1); } ;  

write: WRITE TEXT                           { $$ = add_print_node(add_text_node($2)); }
    | WRITE SYMBOL_NAME                     { $$ = add_print_node(add_variable_reference($2)); } 
    | WRITE write_expression                { $$ = add_print_node($2); };

write_expression: expression BIN_OP expression { $$ = add_expression_node($1, add_text_node($2), $3, 3); };

expression: '(' expression ')'              { $$ = add_expression_node(add_text_node("("), $2, add_text_node(")"), 3); }
    | UNI_OP expression                     { $$ = add_expression_node(add_text_node($1), $2, NULL, 2); }
    | expression BIN_OP expression          { $$ = add_expression_node($1, add_text_node($2), $3, 3); }
    | NUMBER                                { $$ = add_expression_node(add_number_node($1), NULL, NULL, 1); }
    | SYMBOL_NAME                           { $$ = add_expression_node(add_variable_reference($1), NULL, NULL, 1); };

%%

struct variable {
    int variable_type;
    char *variable_name;
    struct variable *next;
};

struct variable *variable_list;


int lookup_variable(char *name){
    struct variable *vp = variable_list;
    for(; vp; vp = vp->next) {
        if(strcmp(vp->variable_name, name) == 0)
            return vp->variable_type;
    }
    return -1;
}

void declare_variable(int type, char *name) {
    struct variable *vp;

    // printf("DEBUG: %s is %d", name, type);

    if(lookup_variable(name) != -1){
        printf("ERROR: Variable '%s' already defined\n", name);
        exit(-1);
    }

    vp = (struct variable *) malloc(sizeof(struct variable));
    vp->next = variable_list;

    vp->variable_name = (char *) malloc(strlen(name)+1);
    strcpy(vp->variable_name, name);

    vp->variable_type = type;

    variable_list = vp;
}

void assign_variable(char * name, char * value, int attr_type) {
    int type = lookup_variable(name);
    if (type == -1) {
        ERROR("'%s' is not defined\n", name);
    }
    if (type == NUMBER_TYPE && atof(value) == 0 && attr_type == TEXT) {
        ERROR("Can't assign text to %s\n", name);
    }
}

char * type_desc(int type) {
    if (type == TEXT_TYPE) return "text";
    if (type == NUMBER_TYPE) return "number";
    if (type == BOOLEAN_TYPE) return "boolean";
    if (type == ARRAY_TYPE) return "array";
    return NULL;
}

void assert_type(int expected_type, char * var) {
    int type = lookup_variable(var);
    if(type == -1) {
        ERROR("'%s' is not defined", var);
    }
    if(type != expected_type) {
        ERROR("'%s' must be %s", var, type_desc(expected_type));
    }
}

void write_symbol(char * name) {
    int type = lookup_variable(name);
    // printf("DEBUG: %s is %d", name, type);
    if(type == -1) {
        ERROR("'%s' is not defined", name);
    }
    if(type == NUMBER_TYPE) {
        P("printf(\"%%f\\n\", (double) (%s));\n", name);
    }
    if(type == TEXT_TYPE) {
        P("printf(\"%%s\\n\", %s);\n", name);
    }
}

void write_expression(char * exp1, char * op, char * exp2) {
    P("printf(\"%%f\\n\", (double) (%s %s %s));\n", exp1, op, exp2);
}
