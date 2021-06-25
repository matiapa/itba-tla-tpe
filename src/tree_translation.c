#include "../include/tree.h"
#include "../include/tree_translation.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P(...) fprintf(out, ##__VA_ARGS__);
FILE * out;

/*-------------------- FUNCIONES ---------------------*/
void print_print(node_t * node);
void print_var(node_t * node);

void read_tree(node_list * program, FILE * file) {
    
    #ifdef YYDEBUG
    printf("Empezando con el translate de código\n");
    #endif
    out = file;
    node_list * aux = program;

    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        switch(nodo->instruction->type) {
            case VARIABLE_NODE:
                print_var(nodo->instruction);
                break;
            case PRINT_NODE:
                print_print(nodo->instruction);
                break;
            default:
                #ifdef YYDEBUG
                printf("Algo salio mal\n");
                #endif                
                break;
        }
        aux = aux->next;
    }

}

void print_var(node_t * node) {
    variable_node * var = (variable_node *) node;
    expression_node * exp = (expression_node *)(var->value);

    if (var->declared == TRUE) {
        switch(var->var_type) {
            case NUMBER_TYPE:
                P("double ");
                break;
            case TEXT_TYPE:
                P("char * ");
                break;
            case ARRAY_TYPE:
                P("double * ");
                break;
            case BOOLEAN_TYPE:
                P("char ");
                break;
            default:
                break;
        }
    }
    P("%s", var->name);
    if (exp != NULL) {
        P(" = %s", exp->expression);
    }
    P(";\n");
}

void print_print(node_t * node) {
    print_node * print = (print_node *) node;
    // no esta funcionando para variables porque esta faltando el lookup del tipo de variable
    // el lookup deberia rellenar el valor de var_type
    if (print->content->type == VARIABLE_NODE) { 
        variable_node * var = (variable_node *)(print->content);
        if (var->var_type == NUMBER_TYPE) {
            P("printf(\"%%f\\n\", (double) (%s));\n", var->name);
        }
        if(var->var_type == TEXT_TYPE) {
            P("printf(\"%%s\\n\", %s);\n", var->name);
        }
    }
    if (print->content->type == EXPRESSION_NODE) {
        expression_node * exp = (expression_node *)print->content;
        if (exp->expression_type == TEXT) {
            P("printf(\"%%s\\n\", %s);\n", exp->expression);
        }
        if (exp->expression_type == EXPRESSION) {
            P("printf(\"%%f\\n\", (double) (%s));\n", exp->expression);
        }
    }
}
