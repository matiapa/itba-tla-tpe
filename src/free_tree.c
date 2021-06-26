#include "../include/tree.h"
#include "../include/free_tree.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------- FUNCIONES ---------------------*/
void free_write(node_t * node);
void free_variable(node_t * node);
void free_expression(node_t * node);
void free_list(node_list * list);
void free_if_node(node_t * node);
void free_while_node(node_t * node);
void free_text_node(node_t * node);
void free_instruction_list(node_list * list);
void free_number_node(node_t * node);
void free_operation_node(node_t * node);
void free_instruction_node(node_t * node);

void free_tree(node_list * program) {
    
    #ifdef YYDEBUG
    printf("Empezando con el translate de código\n");
    #endif

    if (program == NULL)
        return;

    switch (program->type) {
        case LIST_NODE:
            free_instruction_list(program);
            break;
        case INSTRUCTION_NODE:
            free_instruction_node((node_t *)program);
        case BLOCK_NODE:
            free_instruction_list((node_list *)(((block_node *)program)->node_list));
            free(program);
            break;
        case VARIABLE_NODE:
            free_variable((node_t *)program);
            break;
        case EXPRESSION_NODE:
            free_expression((node_t *)program);
            break;
        case TEXT_NODE:
            free_text_node((node_t *)program);
            break;
        case NUMBER_NODE:
            free_number_node((node_t *)program);
            break;
        case WHILE_NODE:
            free_while_node((node_t *)program);
            break;
        case IF_NODE:
            free_if_node((node_t *)program);
            break;
        case OPERATION_NODE:
            free_operation_node((node_t *)program);
            break;
        default:
            break;
    }
    
    free(program);

}

void free_instruction_node(node_t * node) {
    instruction_node * nodo = (instruction_node *)node;
    switch(nodo->instruction->type) {
        case VARIABLE_NODE:
            free_variable(nodo->instruction);
            break;
        case PRINT_NODE:
            free_write(nodo->instruction);
            break;
        case IF_NODE:
            free_if_node(nodo->instruction);
            break;
        case WHILE_NODE:
            free_while_node(nodo->instruction);
            break;
        default:
            #ifdef YYDEBUG
            printf("Algo salio mal\n");
            #endif                
            break;
    }
    free(nodo->instruction);
}

void free_instruction_list(node_list * list) {
    node_list * aux = list;
    while (aux != NULL) {
        free_instruction_node(aux->node);
        free(aux->node);
        node_list * next = aux->next;
        free(aux);
        aux = next;
    }
}

void free_variable(node_t * node) {
    variable_node * var = (variable_node *) node;
    
    free(var->name);
    if (var->value != NULL) {
        if (var->value->type == EXPRESSION_NODE) {
            free_expression(var->value);
        } else if (var->value->type == TEXT_NODE) {
            free_text_node(var->value);
        }
    }
    free(var->value);
}

void free_text_node(node_t * node) {
    free(((text_node *) node)->text);
}

void free_number_node(node_t * node) {
    free(((number_node *) node)->number);
}

void free_operation_node(node_t * node) {
    free(((operation_node *) node)->operation);
}

void free_write(node_t * node) {
    print_node * print = (print_node *) node;
    switch(print->content->type) {
        case VARIABLE_NODE:
            free_variable(print->content);
            break;
        case EXPRESSION_NODE:
            free_expression(print->content);
            break;
        case TEXT_NODE:
            free_text_node(print->content);
            break;
        default:
            break;
    }
    free(print->content);
}

void switch_free_expression(node_t * node) {
    switch (node->type) {
        case EXPRESSION_NODE:
            free_expression(node);
            break;
        case VARIABLE_NODE:
            free_variable(node);
            break;
        case TEXT_NODE:
            free_text_node(node);
            break;
        case NUMBER_NODE:
            free_number_node(node);
            break;
        case OPERATION_NODE:
            free_operation_node(node);
            break;
        default:
            break;
    }
}

void free_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;

    if (node->first != NULL) {
        switch_free_expression(node->first);
        free(node->first);
    }
    if (node->second != NULL) {
        switch_free_expression(node->second);
        free(node->second);
    }
    if (node->third != NULL) {
        switch_free_expression(node->third);
        free(node->third);
    }
}

void free_if_node(node_t * node) {
    if_node * aux = (if_node *)node;
    free_expression(aux->condition);
    free(aux->condition);
    block_node * block = (block_node *)aux->then;
    free_instruction_list((node_list *)block->node_list);
    free(block);
    if (aux->otherwise != NULL) {
        block = (block_node *)aux->otherwise;
        free_instruction_list((node_list *)block->node_list);
        free(block);
    }
}

void free_while_node(node_t * node) {
    while_node * aux = (while_node *)node;
    free_expression(aux->condition);
    free(aux->condition);
    block_node * block = (block_node *)aux->then;
    free_instruction_list((node_list *)block->node_list);
    free(block);
}
