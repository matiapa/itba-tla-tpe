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
void print_expression(node_t * node);
void print_variable(node_t * node);
void read_instruction_list(node_list * list);
void print_if_node(node_t * node);

void read_tree(node_list * program, FILE * file) {
    
    #ifdef YYDEBUG
    printf("Empezando con el translate de código\n");
    #endif
    out = file;

    read_instruction_list(program);

}

void read_instruction_list(node_list * list) {
    node_list * aux = list;
    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        switch(nodo->instruction->type) {
            case VARIABLE_NODE:
                print_var(nodo->instruction);
                break;
            case PRINT_NODE:
                print_print(nodo->instruction);
                break;
            case IF_NODE:
                print_if_node(nodo->instruction);
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
        P(" = ");
        if (var->value->type == EXPRESSION_NODE) {
            print_expression(var->value);
        } else if (var->value->type == TEXT_NODE) {
            text_node * text = (text_node *)var->value;
            P("%s", text->text);
        }
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
        P("printf(\"%%f\\n\", (double) ");
        print_expression(print->content);
        P(");\n");
    }
}

void print_variable(node_t * node) {
    variable_node * var = (variable_node *)node;
    if (var->var_type == NUMBER_TYPE) {
        P("printf(\"%%f\\n\", (double) (%s));\n", var->name);
    }
    if(var->var_type == TEXT_TYPE) {
        P("printf(\"%%s\\n\", %s);\n", var->name);
    }
}

void print_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;

    if (node->first != NULL && node->cant > 0) {
        switch (node->first->type) {
            case EXPRESSION_NODE:
                print_expression(node->first);
                break;
            case VARIABLE_NODE:
                ;
                variable_node * var = (variable_node *)node->first;
                P(" %s ", var->name);
                break;
            case TEXT_NODE:
                P(" %s ", ((text_node *)node->first)->text);
                break;
            case NUMBER_NODE:
                P(" %s ", ((number_node *)node->first)->number);
                break;
            case OPERATION_NODE:
                P(" %s ", ((operation_node *)node->first)->operation);
                break;
            default:
                break;
        }
    }
    if (node->second != NULL && node->cant > 1) {
        switch (node->second->type) {
            case EXPRESSION_NODE:
                print_expression(node->second);
                break;
            case VARIABLE_NODE:
                ;
                variable_node * var = (variable_node *)node->first;
                P(" %s ", var->name);
                break;
            case TEXT_NODE:
                P(" %s ", ((text_node *)node->second)->text);
                break;
            case NUMBER_NODE:
                P(" %s ", ((number_node *)node->second)->number);
                break;
            case OPERATION_NODE:
                P(" %s ", ((operation_node *)node->second)->operation);
                break;
            default:
                break;
        }
    }
    if (node->third != NULL && node->cant > 2) {
        switch (node->third->type) {
            case EXPRESSION_NODE:
                print_expression(node->third);
                break;
            case VARIABLE_NODE:
                ;
                variable_node * var = (variable_node *)node->first;
                P(" %s ", var->name);
                break;
            case TEXT_NODE:
                P(" %s ", ((text_node *)node->third)->text);
                break;
            case NUMBER_NODE:
                P(" %s ", ((number_node *)node->third)->number);
                break;
            case OPERATION_NODE:
                P(" %s ", ((operation_node *)node->third)->operation);
                break;
            default:
                break;
        }
    }
}

void print_if_node(node_t * node) {
    if_node * aux = (if_node *)node;
    P("if (");
    print_expression(aux->condition);
    P(") {\n");
    block_node * block = (block_node *)aux->then;
    read_instruction_list((node_list *)block->node_list);
    if (aux->otherwise != NULL) {
        P("\n} else {\n");
        block = (block_node *)aux->otherwise;
        read_instruction_list((node_list *)block->node_list);
    }
    P("}\n");
}
