%{
#include "y.tab.h"
extern int yylval;

%}
%%

^number     { return NUMBER_TYPE; }
^text       { return TEXT_TYPE; }
^boolean    { return BOOLEAN_TYPE; }
^list       { return ARRAY_TYPE; }
^write      { return WRITE; }

[a-zA-Z_]+  { return TEXT; }
[0-9]+      { yylval = yytext; return NUMBER; }

.           { return yytext[0]; };

%%