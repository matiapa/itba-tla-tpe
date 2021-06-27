// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/tree.h"
#include "../include/tree_translation.h"
#include "../include/var_finder.h"
#include "../include/free_tree.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>

int yyparse(node_list ** program);
extern int yylineno;

extern FILE * yyin;
FILE * out;
node_list * program;

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
    
    fprintf(out, "#include \"include/temp.h\"\n");
    fprintf(out, "int main() {\n");

    yyparse(&program);
    if (program->node == NULL) {
        free(program);
        program = NULL;
    }
    if (check_and_set_variables(program)==-1) {
        printf("failure occured while checking variable validity\n");
        exit(-1);
    }
    
    read_tree(program, out);

    fprintf(out, "return 0;\n");
    fprintf(out, "\n}");
    fclose(out);

    system("gcc src/functions.c temp.c -lm -o program");

    printf("\nSuccesfully parsed\n");

}

void yyerror(node_list ** param, char *s){
    printf("%s at line %d\n", s, yylineno);

    free_tree(program);

    exit(1);
}
