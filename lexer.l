%{
#include "y.tab.h"
#include <stdio.h>
int fileno(FILE * stream);
%}
%%

^number     { yylval.number = NUMBER_TYPE; return NUMBER_TYPE; }
^text       { yylval.number = TEXT_TYPE; return TEXT_TYPE; }
^boolean    { yylval.number = BOOLEAN_TYPE; return BOOLEAN_TYPE; }
^list       { yylval.number = ARRAY_TYPE; return ARRAY_TYPE; }
^write      { return WRITE; }

\"[^\"]+\"                  { strcpy(yylval.string, yytext); return TEXT; }
[-]?[0-9]+(\.[0-9]+)?       { strcpy(yylval.string, yytext); return NUMBER; }

[-+\*/%]                  { strcpy(yylval.string, yytext); return BIN_OP; }
[-~]                        { strcpy(yylval.string, yytext); return UNI_OP; }
[<>]                        { return yytext[0]; }
"<="                        { strcpy(yylval.string, yytext); return LEQ; }
">="                        { strcpy(yylval.string, yytext); return GEQ; }
"neq"		                { strcpy(yylval.string, yytext); return NEQ; }
"eq"		                { strcpy(yylval.string, yytext); return EQ; }
"and"		                { strcpy(yylval.string, yytext); return AND; }
"or"		                { strcpy(yylval.string, yytext); return OR; }
"not"		                { strcpy(yylval.string, yytext); return NOT; }

[a-zA-Z_][a-zA-Z0-9_]*      { strcpy(yylval.string, yytext); return SYMBOL_NAME; }

[ \t] ;

.           { return yytext[0]; };

[\n]        { return EOL; };

%%