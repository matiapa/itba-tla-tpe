%{
#include "include/tree.h"
#include "y.tab.h"
#include <stdio.h>
int fileno(FILE * stream);
%}

%option yylineno

DECIMAL [-]?[0-9]+(\.[0-9]+)?

%%
start      { return START;}
number     { yylval.number = NUMBER_TYPE; return NUMBER_TYPE; }
text       { yylval.number = TEXT_TYPE; return TEXT_TYPE; }
boolean    { yylval.number = BOOLEAN_TYPE; return BOOLEAN_TYPE; }
list       { yylval.number = LIST_TYPE; return LIST_TYPE; }
write      { return WRITE; }

(mean|median|mode|stdev|range|qtr1|qtr3|inter_qtr)[ ]of {
    int i; for(i=0; yytext[i]!=' '; i++);
    memcpy(yylval.string, yytext, i); yylval.string[i]=0;
    return FUNCTION_CALL;
}

\"[^\"]+\"                  { strcpy(yylval.string, yytext); return TEXT; }
{DECIMAL}                   { strcpy(yylval.string, yytext); return NUMBER; }
\[[ \t]*{DECIMAL}([ \t]*,[ \t]*{DECIMAL})*[ \t]*\]  { strcpy(yylval.string, yytext); return LIST; }

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
