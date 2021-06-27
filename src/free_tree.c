#include "../include/tree.h"
#include "../include/free_tree.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void * free_tree(node_list * program) {

    if (program == NULL)
        return NULL;

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
    return NULL;
}

void * free_instruction_node(node_t * node) {
    instruction_node * nodo = (instruction_node *)node;
    if (nodo == NULL)
        return NULL;
    switch(nodo->instruction->type) {
        case VARIABLE_NODE:
            free_variable(nodo->instruction);
            break;
        case PRINT_NODE:
            free_write(nodo->instruction);
            break;
        case READ_NODE:
            free_read(nodo->instruction);
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
    free(node);
    return NULL;
}

void * free_instruction_list(node_list * list) {
    node_list * aux = list;
    while (aux != NULL) {
        free_instruction_node(aux->node);
        node_list * next = aux->next;
        free(aux);
        aux = next;
    }
    return NULL;
}

void * free_variable(node_t * node) {
    variable_node * var = (variable_node *) node;
    
    free(var->name);
    if (var->value != NULL) {
        if (var->value->type == EXPRESSION_NODE) {
            free_expression(var->value);
        } else if (var->value->type == TEXT_NODE) {
            free_text_node(var->value);
        } else if (var->value->type == ARRAY_NODE) {
            free_array_node(var->value);
        }
    }
    free(node);
    return NULL;
}

void * free_text_node(node_t * node) {
    free(((text_node *) node)->text);
    free(node);
    return NULL;
}

void * free_number_node(node_t * node) {
    free(((number_node *) node)->number);
    free(node);
    return NULL;
}

void * free_operation_node(node_t * node) {
    free(((operation_node *) node)->operation);
    free(node);
    return NULL;
}

void * free_array_node(node_t * node) {
    free(((array_node *) node)->array);
    free(node);
    return NULL;
}

void * free_write(node_t * node) {
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
        case ARRAY_NODE:
            free_array_node(print->content);
            break;
        default:
            break;
    }
    free(node);
    return NULL;
}

void * free_read(node_t * node) {
    read_node * read = (read_node *) node;
    switch(read->content->type) {
        case VARIABLE_NODE:
            free_variable(read->content);
            break;
        default:
            break;
    }
    free(node);
    return NULL;
}

void * switch_free_expression(node_t * node) {
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
        case LIST_OP_NODE:
            free_list_op(node);
            break;
        default:
            break;
    }
    return NULL;
}

void * free_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;

    if (node->first != NULL) {
        switch_free_expression(node->first);
    }
    if (node->second != NULL) {
        switch_free_expression(node->second);
    }
    if (node->third != NULL) {
        switch_free_expression(node->third);
    }
    free(node);
    return NULL;
}

void * free_if_node(node_t * node) {
    if_node * aux = (if_node *)node;
    free_expression(aux->condition);
    block_node * block = (block_node *)aux->then;
    free_instruction_list((node_list *)block->node_list);
    free(block);
    if (aux->otherwise != NULL) {
        block = (block_node *)aux->otherwise;
        free_instruction_list((node_list *)block->node_list);
        free(block);
    }
    free(node);
    return NULL;
}

void * free_while_node(node_t * node) {
    while_node * aux = (while_node *)node;
    free_expression(aux->condition);
    block_node * block = (block_node *)aux->then;
    free_instruction_list((node_list *)block->node_list);
    free(block);
    return NULL;
}

void * free_list_op(node_t * node) {
    list_op_node * lop = (list_op_node *)node;

    if (lop->list->type == ARRAY_NODE) {
        free_array_node(lop->list);
    } else if (lop->list->type == VARIABLE_NODE) {
        free_variable(lop->list);
    }
    free(lop->list);
    free(lop->operator);
    free(node);
    return NULL;
}
