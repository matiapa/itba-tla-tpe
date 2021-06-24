%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yydebug=1;
int yylex();
void yyerror(char *s);

extern FILE * out;
extern void * malloc();

int lookup_variable(char * name);
void declare_variable(int type, char * name);
void assign_variable(char * name, char * value);

#define P(...) fprintf(out, ##__VA_ARGS__);

%}

%union {
    char string[1024];
    double array[1024];
    double number;
    char boolean;
}

%token START
%token ADDITION SUBSTRACTION DIVISION MULTIPLICATION MOD POWER
%token LT GT LEQ GEQ NEQ EQ
%token AND OR NOT
%token <string> NUMBER 
%token <string> TEXT
%token <string> BOOLEAN 
%token <string> ARRAY 
%token MEAN MEDIAN MODE STDEV RANGE QTR1 QTR3 INTER_QTR GCD MCM
%token COMB PERM
%token FACT
%token OF
%token ASSIGN
%token IF WHILE DO END ELSE
%token EOL
%token TEXT_TYPE NUMBER_TYPE BOOLEAN_TYPE ARRAY_TYPE

%token <string> SYMBOL_NAME
%token WRITE

%type <number> type
%type <string> declare value

%%

program: instruction program | instruction;
instruction: full_declare EOL | assign EOL | write EOL;

full_declare: declare '=' value { assign_variable($1, $3); P(" %s = %s;\n", $1, $3); }
    | declare { P(" %s;\n", $1); };

declare: type SYMBOL_NAME { strcpy($$, $2); declare_variable($1, $2); }

type: NUMBER_TYPE   { P("double")  }
    | TEXT_TYPE     { P("char *")  }
    | BOOLEAN_TYPE  { P("char")    }
    | ARRAY_TYPE    { P("double *")};

assign: SYMBOL_NAME '=' value { assign_variable($1, $3); P("%s = %s;\n", $1, $3); };
value: NUMBER | TEXT ;

write: WRITE TEXT   { P("printf(\"%%s\", %s);\n", $2); }
    | WRITE NUMBER  { P("printf(\"%%s\", \"%s\");\n", $2); };

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

    printf("DEBUG: Adding variable %s of type %d\n", name, type);

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

void assign_variable(char * name, char * value) {
    int type = lookup_variable(name);

    if (type == -1) {
        printf("ERROR: Variable '%s' is not defined\n", name);
        exit(-1);
    }

    if (type == NUMBER_TYPE && atof(value) == 0) {
        printf("ERROR: Can't assign text to %s\n", name);
        exit(-1);
    }
}