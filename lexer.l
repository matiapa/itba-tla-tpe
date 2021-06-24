%{
#include "y.tab.h"

%}
%%

^number     { return NUMBER_TYPE; }
^text       { return TEXT_TYPE; }
^boolean    { return BOOLEAN_TYPE; }
^list       { return ARRAY_TYPE; }
^write      { return WRITE; }

[a-zA-Z_]+  { strcpy(yylval.string, yytext); return TEXT; }
[0-9]+      { strcpy(yylval.string, yytext); return NUMBER; }

[ \t] ;

.           { printf("--%c--", yytext[0]); return yytext[0]; };

[\n]        { return EOL; };

%%