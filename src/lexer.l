%{
#include "include/tree.h"
#include "y.tab.h"
#include <stdio.h>
int fileno(FILE * stream);
%}
%%

number     { yylval.number = NUMBER_TYPE; return NUMBER_TYPE; }
text       { yylval.number = TEXT_TYPE; return TEXT_TYPE; }
boolean    { yylval.number = BOOLEAN_TYPE; return BOOLEAN_TYPE; }
list       { yylval.number = ARRAY_TYPE; return ARRAY_TYPE; }
write      { return WRITE; }

\"[^\"]+\"                  { strcpy(yylval.string, yytext); return TEXT; }
[-]?[0-9]+(\.[0-9]+)?       { strcpy(yylval.string, yytext); return NUMBER; }
\{([0-9],)+\}                 { strcpy(yylval.string, yytext); return ARRAY; }

[-+\*/%<>]|<=|>=            { strcpy(yylval.string, yytext); return BIN_OP; }
"neq"		                { strcpy(yylval.string, "!="); return BIN_OP; }
"eq"		                { strcpy(yylval.string, "=="); return BIN_OP; }
"and"		                { strcpy(yylval.string, "&&"); return BIN_OP; }
"or"		                { strcpy(yylval.string, "||"); return BIN_OP; }
"not"		                { strcpy(yylval.string, "!"); return BIN_OP; }
[-~]                        { strcpy(yylval.string, yytext); return UNI_OP; }

[a-zA-Z_][a-zA-Z0-9_]*      { strcpy(yylval.string, yytext); return SYMBOL_NAME; }

#.*                         ; // Ignore comments
[ \t]                       ; // Ignore whitespaces
<<EOF>>                     { static int once = 0; return once++ ? 0 : FIN; }

.           { return yytext[0]; };
[\n]        { return EOL; };

%%