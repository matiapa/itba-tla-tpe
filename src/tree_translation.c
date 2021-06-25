#include "../include/tree.h"
#include "../include/tree_translation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P(...) fprintf(out, ##__VA_ARGS__);
FILE * out;

void read_tree(node_list * program, FILE * file) {
    out = file;
    node_list * aux = program;

    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        switch(nodo->instruction->type) {
            case VARIABLE_NODE:
                print_var(nodo->instruction);
                break;
            default:
                printf("Algo salio mal\n");
                break;
        }
        aux = aux->next;
    }

}

void print_var(node_t * node) {
    variable_node * var = (variable_node *) node;
    expression_node * exp = (expression_node *)(var->value);

    if (var->var_type == NULL) {
        P("%s = %s;\n", var->name, exp->expression);
    } else {
        P("%s %s = %s;\n", var->var_type, var->name, exp->expression);
    }
}
