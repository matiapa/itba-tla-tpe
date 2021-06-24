#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int yyparse();

FILE * out;

int main(void) {

    out = fopen("temp.c", "w+");
    if (out == NULL) {
        perror("Creating temporary file: ");
        exit(-1);
    }
    
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "int main() {\n");

    yyparse();

    fprintf(out, "\n}");
    fclose(out);

    system("gcc temp.c -o program");

    printf("\nSuccesfully parsed\n");

}


void yyerror(char *s){

    printf("\nSyntax error\n");

    exit(-1);

}