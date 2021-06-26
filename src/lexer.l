%{
#include "include/tree.h"
#include "include/functions.h"
#include "y.tab.h"
#include <stdio.h>
int fileno(FILE * stream);
%}

%option yylineno

%%
start      { return START;}
number     { yylval.number = NUMBER_TYPE; return NUMBER_TYPE; }
text       { yylval.number = TEXT_TYPE; return TEXT_TYPE; }
boolean    { yylval.number = BOOLEAN_TYPE; return BOOLEAN_TYPE; }
list       { yylval.number = LIST_TYPE; return LIST_TYPE; }
write      { return WRITE; }

(mean|median|mode|stdev|range|qtr1|qtr3|inter-qtr|perc-n|gcd|mcm)[ ]of {
    strcpy(yylval.string, str_to_fun(yytext)); return FUNCTION_CALL;
}

\"[^\"]+\"                  { strcpy(yylval.string, yytext); return TEXT; }
[-]?[0-9]+(\.[0-9]+)?       { strcpy(yylval.string, yytext); return NUMBER; }
\[[ \t]*[0-9]([ \t]*,[ \t]*[0-9])*[ \t]*\]  { strcpy(yylval.string, yytext); return LIST; }

=                           { return ASSIGN;     }

[-+\*/%<>]|<=|>=            { strcpy(yylval.string, yytext); return BIN_OP; }
neq		                    { strcpy(yylval.string, "!="); return BIN_OP; }
eq		                    { strcpy(yylval.string, "=="); return BIN_OP; }
and		                    { strcpy(yylval.string, "&&"); return BIN_OP; }
or		                    { strcpy(yylval.string, "||"); return BIN_OP; }
not		                    { strcpy(yylval.string, "!"); return BIN_OP; }
[-~]                        { strcpy(yylval.string, yytext); return UNI_OP; }
if                          { strcpy(yylval.string, yytext); return IF; }
else                        { strcpy(yylval.string, yytext); return ELSE; }
while                       { strcpy(yylval.string, yytext); return WHILE; }
do                          { strcpy(yylval.string, yytext); return DO; }
end                         { return END; }

[a-zA-Z_][a-zA-Z0-9_]*      { strcpy(yylval.string, yytext); return SYMBOL_NAME; }

#.*                         ; // Ignore comments
[ \t]                       ; // Ignore whitespaces
<<EOF>>                     { static int once = 0; return once++ ? 0 : FIN; }

.           { return yytext[0]; };
[\n]        { return EOL; };

%%
