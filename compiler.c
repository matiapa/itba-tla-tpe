#include "tree.h"
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int yyparse(node_list ** program);

extern FILE * yyin;
FILE * out;

void print_var(node_t * node) {
    variable_node * var = (variable_node *) node;
    expression_node * exp = (expression_node *)(var->value);

    printf("%s %s = %s;\n", var->var_type, var->name, exp->expression);
}

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

    instruction_node * instruction = (instruction_node *)program->node;
    // variable_node * variable = (variable_node *)instruction->instruction;

    //print_var(instruction->instruction);


    fprintf(out, "return 0;\n");
    fprintf(out, "\n}");
    fclose(out);

    system("gcc temp.c -o program");

    printf("\nSuccesfully parsed\n");

}


void yyerror(node_list ** program, char *s){

    printf("%s", s);

    exit(-1);

}
