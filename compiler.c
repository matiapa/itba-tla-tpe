#include "y.tab.h"
#include <stdio.h>

int yyparse();

int main(void) {
    
    fprintf(stderr, "int main() {\n");

    yyparse();

    fprintf(stderr, "\n}");

    printf("\nSuccesfully parsed\n");

}


void yyerror(char *s){

    printf("\nSyntax error\n");

}