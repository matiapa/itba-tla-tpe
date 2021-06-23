%{
#include <stdio.h>

#if YYBISON
int yylex();
#endif
%}

%token START
%token ADDITION SUBSTRACTION DIVISION MULTIPLICATION MOD POWER
%token LT GT LEQ GEQ NEQ EQ
%token AND OR NOT
%token NUMBER TEXT BOOLEAN ARRAY
%token MEAN MEDIAN MODE STDEV RANGE QTR1 QTR3 INTER_QTR GCD MCM
%token COMB PERM
%token FACT
%token OF
%token ASSIGN
%token IF WHILE DO END ELSE
%token EOL
%token NUMBER_TYPE TEXT_TYPE BOOLEAN_TYPE ARRAY_TYPE

%%

expression: '(' '(' NUMBER relation NUMBER ')' complex_expression ')'
            | '(' '(' BOOLEAN ')' complex_expression ')'
            | '(' BOOLEAN ')'
            | '(' NUMBER complex_expression NUMBER ')'
            ;

type: NUMBER
    | TEXT
    | BOOLEAN
    ;

relation: LT 
        | GT 
        | LEQ 
        | GEQ 
        | NEQ 
        | EQ
        ;

complex_expression: logical expression complex_expression
                  | logical expression
                  ;

logical: AND 
        | OR 
        | NOT
        ;

%%

void yyerror(char *s){
    printf("Syntax error");
}