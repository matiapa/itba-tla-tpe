#include "../include/tree.h"
#include "../include/tree_translation.h"
#include "../include/var_finder.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse(node_list ** program);

extern FILE * yyin;
FILE * out;

int main(int argc, char ** argv) {
    node_list * program;

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

    yyparse(&program);

    check_and_set_variables(program);

    read_tree(program, out);

    fprintf(out, "return 0;\n");
    fprintf(out, "\n}");
    fclose(out);

    system("gcc temp.c -o program");

    printf("\nSuccesfully parsed\n");

}


void yyerror(node_list ** program, char *s){

    printf("%s", s);
    // printf("%d: %s at '%s'\n", yylineno, msg, yytext);

    exit(-1);

}

// aux es una lista de nodos de tipo LIST_NODE
// LIST_NODE tiene un INSTRUCTION_NODE
// INSTRUCTION_NODE tiene cualquier otro tipo de nodo
