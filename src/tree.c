#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t * add_expression_node(node_t * first, node_t * second, node_t * third, int cant) {
    expression_node * expression = calloc(1, sizeof(expression_node));
    expression->type = EXPRESSION_NODE;
    expression->cant = cant;
    if (first != NULL)
        expression->first = first;
    if (second != NULL)
        expression->second = second;
    if (third != NULL)
        expression->third = third;

    return (node_t *)expression;
}

node_t * declare_variable_node(char * name, int var_type) {
    variable_node * node = calloc(1, sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    node->var_type = var_type;
    node->declared = TRUE;
    strcpy(node->name, name);

    return (node_t *)node;
}

node_t * add_value_variable(node_t * past_node, node_t * expression) {
    variable_node * var_node = (variable_node *) past_node;
    variable_node * node = malloc(sizeof(variable_node));
    node->name = malloc(strlen(var_node->name) + 1);
    strcpy(node->name, var_node->name);
    node->value = expression;
    node->declared = TRUE;
    node->type = VARIABLE_NODE;
    node->var_type = var_node->var_type;

    return (node_t *)node;
}

node_t * add_variable_reference(char * name) {
    variable_node * node = calloc(1, sizeof(variable_node));
    node->name = malloc(strlen(name) + 1);
    strcpy(node->name, name);
    node->declared = FALSE;
    node->type = VARIABLE_NODE;

    return (node_t *)node;
}

node_t * assign_variable_node(char * name, node_t * expression) {
    variable_node * node = calloc(1, sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    node->declared = FALSE;
    node->value = expression;
    strcpy(node->name, name);

    return (node_t *)node;
}

node_t * add_instruction_node(node_t * node) {
    instruction_node * new = malloc(sizeof(instruction_node));
    new->instruction = node;
    new->type = INSTRUCTION_NODE;
    return (node_t *)new;
}

node_t * add_instruction_list_node(node_t * node) {
    node_list * new = malloc(sizeof(node_list));
    new->node = node;
    new->next = NULL;
    new->type = LIST_NODE;
    return (node_t *)new;
}

node_t * add_element_to_list(node_list * list, node_t * element) {
    node_list * current_node = list;
    node_list * new_node = (node_list *)add_instruction_list_node(element);

    if (current_node->node == NULL) {
        free(list);
        return (node_t *)new_node;
    } else {
        new_node->next = current_node;
    }

    return (node_t *)new_node;
}

node_t * add_print_node(node_t * content) {
    print_node * node = malloc(sizeof(print_node));
    node->content = content;
    node->type = PRINT_NODE;
    return (node_t *)node;
}

node_t * add_text_node(char * text) {
    text_node * node = (text_node *)malloc(sizeof(text_node));
    node->text = malloc(strlen(text) + 1);
    strcpy(node->text, text);
    node->type = TEXT_NODE;

    return (node_t *)node;
}

node_t * add_number_node(char * number) {
    number_node * node = (number_node *)malloc(sizeof(number_node));
    node->number = malloc(strlen(number) + 1);
    strcpy(node->number, number);
    node->type = NUMBER_NODE;

    return (node_t *)node;
}

node_t * add_list_node(char * array) {
    array_node * node = (array_node *)malloc(sizeof(array_node));
    node->array = malloc(strlen(array) + 1);
    strcpy(node->array, array);
    node->type = ARRAY_NODE;

    return (node_t *)node;
}
