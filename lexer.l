%{
#include "y.tab.h"
#include <stdio.h>
int fileno(FILE * stream);
%}
%%

^number     { return NUMBER_TYPE; }
^text       { return TEXT_TYPE; }
^boolean    { return BOOLEAN_TYPE; }
^list       { return ARRAY_TYPE; }
^write      { return WRITE; }

[a-zA-Z_][a-zA-Z0-9_]*      { strcpy(yylval.string, yytext); return SYMBOL_NAME; }
\"[^\"]+\"                  { strcpy(yylval.string, yytext); return TEXT; }
[-]?[0-9]+(\.[0-9]+)?       { strcpy(yylval.string, yytext); return NUMBER; }

[-+\*/<>%]|[<>]=            { strcpy(yylval.string, yytext); return BIN_OP; }
[-~]                        { strcpy(yylval.string, yytext); return UNI_OP; }

[ \t] ;

.           { return yytext[0]; };

[\n]        { return EOL; };

%%