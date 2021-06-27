%{
#include "include/tree.h"
#include "y.tab.h"
#include <stdio.h>

int fileno(FILE * stream);

void put_trimmed(char * str) {
    int i; for(i=0; str[i]!=' '; i++);
    memcpy(yylval.string, str, i);
    yylval.string[i] = 0;
}

extern int reduced;

%}

%option yylineno

DECIMAL [0-9]+(\.[0-9]+)?
SP      [ \t]+

%%
start      { return START;}
number     { yylval.number = NUMBER_TYPE; return NUMBER_TYPE; }
text       { yylval.number = TEXT_TYPE; return TEXT_TYPE; }
boolean    { yylval.number = BOOLEAN_TYPE; return BOOLEAN_TYPE; }
list       { yylval.number = LIST_TYPE; return LIST_TYPE; }
write      { return WRITE; }
read       { return READ; }
size       { return SIZE; }


\"[^\"]+\"                                          { strcpy(yylval.string, yytext); return TEXT; }
e                                                   { strcpy(yylval.string, "2.73"); return NUMBER; }
[E][ ]                                              { strcpy(yylval.string, yytext); return BIN_OP; }
{DECIMAL}                                           { strcpy(yylval.string, yytext); return NUMBER; }
\[[ \t]*{DECIMAL}([ \t]*,[ \t]*{DECIMAL})*[ \t]*\]  { strcpy(yylval.string, yytext); return LIST; }
=                           { return ASSIGN;     }

\^|\*\*                     { strcpy(yylval.string, "^"); return POWER; }
[+\*/%<>]|<=|>=|comb|perm   { strcpy(yylval.string, yytext); return BIN_OP; }
!                           { strcpy(yylval.string, yytext); return FACT; }
mod                         { strcpy(yylval.string, "%"); return BIN_OP; }
neq		                    { strcpy(yylval.string, "!="); return BIN_OP; }
eq		                    { strcpy(yylval.string, "=="); return BIN_OP; }
and		                    { strcpy(yylval.string, "&&"); return BIN_OP; }
or		                    { strcpy(yylval.string, "||"); return BIN_OP; }

not		                    { strcpy(yylval.string, "!"); return UNI_OP; }
~                           { strcpy(yylval.string, "~"); return UNI_OP; }

(mean|median|mode|stdev|range|qtr1|qtr3|iqtr){SP}of { put_trimmed(yytext); return MEASURE_OF; }
in                          { return IN; }

if                          { strcpy(yylval.string, yytext); return IF; }
else                        { strcpy(yylval.string, yytext); return ELSE; }
while                       { strcpy(yylval.string, yytext); return WHILE; }
do                          { strcpy(yylval.string, yytext); return DO; }
end                         { return END; }

[a-zA-Z_][a-zA-Z0-9_]*      { strcpy(yylval.string, yytext); return SYMBOL_NAME; }

#.*                         ; // Ignore comments
{SP}                        ; // Ignore whitespaces
<<EOF>>                     { static int once = 0; return once++ ? 0 : FIN; }

.           { return yytext[0]; };
[\n]        { return EOL; };

%%
