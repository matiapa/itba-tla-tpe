#include "../include/tree.h"
#include "../include/tree_translation.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P(...) fprintf(output, ##__VA_ARGS__);
FILE * output;

/*-------------------- FUNCIONES ---------------------*/

void yyerror(node_list ** program, char *s);

void print_print(node_t * node);
void print_var(node_t * node);
void print_expression(node_t * node);
void read_instruction_list(node_list * list);
void print_if_node(node_t * node);
void print_while_node(node_t * node);
void print_function_call(node_t * node);
void free_text_node(node_t * node);
void free_number_node(node_t * node);
void free_operation_node(node_t * node);

void read_tree(node_list * program, FILE * file) {
    
    #ifdef YYDEBUG
    printf("Empezando con el translate de código\n");
    #endif
    output = file;

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
            case WHILE_NODE:
                print_while_node(nodo->instruction);
                break;
            case FUNCTION_CALL_NODE:
                print_function_call(nodo->instruction);
                break;
            default:
                #ifdef YYDEBUG
                printf("Algo salio mal\n");
                #endif
                break;
        }
        free(nodo->instruction);
        free(nodo);
        node_list * next = aux->next;
        free(aux);
        aux = next;
    }
}

void print_var(node_t * node) {
    variable_node * var = (variable_node *) node;

    if (var->declared == TRUE) {
        switch(var->var_type) {
            case NUMBER_TYPE:
                P("double %s", var->name);
                break;
            case TEXT_TYPE:
                P("char * %s", var->name);
                break;
            case LIST_TYPE:
                if (var->value == NULL || var->value->type == VARIABLE_NODE) {
                    P("double * %s", var->name);
                } else {
                    P("double %s[]", var->name);
                }
                break;
            case BOOLEAN_TYPE:
                P("char %s", var->name);
                break;
            default:
                break;
        }
    }
    else{
        P("%s",var->name);
    }
    free(var->name);

    if (var->value != NULL) {
        P(" = ");
        if (var->value->type == EXPRESSION_NODE) {
            print_expression(var->value);
        } else if (var->value->type == TEXT_NODE) {
            text_node * text = (text_node *) var->value;
            P("%s", text->text);
        } else if (var->value->type == ARRAY_NODE) {
            array_node * array = (array_node *) var->value;
            P("%s", array->array);
        }
        free(var->value);
    }
    P(";\n");
}

void print_print(node_t * node) {
    print_node * print = (print_node *) node;
    // no esta funcionando para variables porque esta faltando el lookup del tipo de variable
    // el lookup deberia rellenar el valor de var_type
    switch(print->content->type) {
        case VARIABLE_NODE:
            ;
            variable_node * var = (variable_node *)(print->content);
            if (var->var_type == NUMBER_TYPE)
                P("printf(\"%%f\\n\", (double) (%s));\n", var->name);
            if(var->var_type == TEXT_TYPE)
                P("printf(\"%%s\\n\", %s);\n", var->name);
            free(var->name);
            break;
        case EXPRESSION_NODE:
            P("printf(\"%%f\\n\", (double) (");
            print_expression(print->content);
            P("));\n");
            break;
        case TEXT_NODE:
            ;
            text_node * text = (text_node *)print->content;
            P("printf(\"%%s\\n\", %s);\n", text->text);
            free(text->text);
            break;
        default:
            break;
    }
    free(print->content);
}

void switch_print_expression(node_t * node) {
    switch (node->type) {
        case EXPRESSION_NODE:
            print_expression(node);
            break;
        case VARIABLE_NODE:
            ;
            variable_node * var = (variable_node *)node;
            P(" %s ", var->name);
            free(var->name);
            break;
        case TEXT_NODE:
            P(" %s ", ((text_node *)node)->text);
            free(((text_node *)node)->text);
            break;
        case NUMBER_NODE:
            P(" %s ", ((number_node *)node)->number);
            free(((number_node *)node)->number);
            break;
        case OPERATION_NODE:
            P(" %s ", ((operation_node *)node)->operation);
            free(((operation_node *)node)->operation);
            break;
        default:
            break;
    }
}

void print_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;

    if (node->first != NULL) {
        switch_print_expression(node->first);
        free(node->first);
    }
    if (node->second != NULL) {
        switch_print_expression(node->second);
        free(node->second);
    }
    if (node->third != NULL) {
        switch_print_expression(node->third);
        free(node->third);
    }
}

void print_if_node(node_t * node) {
    if_node * aux = (if_node *)node;
    P("if (");
    print_expression(aux->condition);
    free(aux->condition);
    P(") {\n");
    block_node * block = (block_node *)aux->then;
    read_instruction_list((node_list *)block->node_list);
    free(block);
    if (aux->otherwise != NULL) {
        P("\n} else {\n");
        block = (block_node *)aux->otherwise;
        read_instruction_list((node_list *)block->node_list);
        free(block);
    }
    P("}\n");
}

void print_while_node(node_t * node) {
    while_node * aux = (while_node *)node;
    P("while (");
    print_expression(aux->condition);
    free(aux->condition);
    P(") {\n");
    block_node * block = (block_node *)aux->then;
    read_instruction_list((node_list *)block->node_list);
    free(block);
    P("}\n");
}

void print_function_call(node_t * node) {
    function_call_node * fcall = (function_call_node *)node;

    if (fcall->input_list->type == ARRAY_NODE) {
        char * str = ((array_node *) fcall->input_list)->array;
        P("str_caller(%s,%s)", str, fcall->function_name);
    } else {
        variable_node * symbol = (variable_node *) fcall->input_list;
        if (symbol->type != LIST_TYPE)
            yyerror(NULL, "Invalid type");
        P("%s(%s)", fcall->function_name, symbol->name);
    }
}
