#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int yyparse();

extern FILE * yyin;
FILE * out;

int main(int argc, char ** argv) {

    if (argc == 1) {
        printf("Reading program from stdin\n");
    } else if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            perror("Error opening input file");
            exit(-1);
        }
    } else {
        printf("Usage: %s [file]\n", argv[0]);
        exit(-1);
    }

    out = fopen("temp.c", "w+");
    if (out == NULL) {
        perror("Error creating temporary file");
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

    printf("%s", s);

    exit(-1);

}
