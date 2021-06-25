#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t * add_expression(char * expression) {
    expression_node * expression_node = malloc(sizeof(expression_node));
    expression_node->type = EXPRESSION_NODE;
    expression_node->expression = malloc(strlen(expression)+1);
    strcpy(expression_node->expression, expression);

    return (node_t *)expression_node;
}

node_t * declare_variable_node(char * name, char * var_type) {
    variable_node * node = calloc(1, sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    node->var_type = malloc(strlen(var_type) + 1);
    node->declared = FALSE;
    strcpy(node->name, name);
    strcpy(node->var_type, var_type);

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
    node->var_type = malloc(strlen(var_node->var_type) + 1);
    strcpy(node->var_type, var_node->var_type);

    return (node_t *)node;
}


// TODO: buscar el nodo de la variable para obtener el tipo de variable
node_t * assign_variable_node(char * name, node_t * expression) {
    variable_node * node = calloc(1, sizeof(variable_node));
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    node->declared = TRUE;
    node->value = expression;
    strcpy(node->name, name);

    return (node_t *)node;
}

node_t * add_instruction_node(node_t * node) {
    instruction_node * new = malloc(sizeof(instruction_node));
    variable_node * var = (variable_node *)node;
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
    instruction_node * inst = (instruction_node *)element;
    variable_node * var = (variable_node *)inst->instruction;

    node_list * new_node = (node_list *)add_instruction_list_node(element);

    if (current_node->node == NULL) {
        free(list);
        return (node_t *)new_node;
    } else {

        new_node->next = current_node;
        // while (current_node->next != NULL)
        //     current_node = (node_list *)current_node->next;
        // current_node->next = new_node;
    }

    return (node_t *)new_node;
}
