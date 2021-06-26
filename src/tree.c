#include "../include/tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t * add_function_call(char * function_name, node_t * input_list) {
    function_call_node * node = malloc(sizeof(function_call_node));

    node->type = FUNCTION_CALL_NODE;
    node->function_name = malloc(strlen(function_name));
    node->input_list = input_list;
    strcpy(node->function_name, function_name);

    return (node_t *) node;
}

node_t * add_expression_node(node_t * first, node_t * second, node_t * third) {
    expression_node * expression = calloc(1, sizeof(expression_node));
    if (expression == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    expression->type = EXPRESSION_NODE;
    expression->first = first;
    expression->second = second;
    expression->third = third;

    return (node_t *)expression;
}

node_t * declare_variable_node(char * name, int var_type) {
    variable_node * node = calloc(1, sizeof(variable_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    if (node->name == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->var_type = var_type;
    node->declared = TRUE;
    strcpy(node->name, name);

    return (node_t *)node;
}

node_t * add_value_variable(node_t * past_node, node_t * expression) {
    variable_node * var_node = (variable_node *) past_node;
    variable_node * node = malloc(sizeof(variable_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->name = malloc(strlen(var_node->name) + 1);
    if (node->name == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->name, var_node->name);
    node->value = expression;
    node->declared = TRUE;
    node->type = VARIABLE_NODE;
    node->var_type = var_node->var_type;

    free(var_node->name);
    free(var_node);
    return (node_t *)node;
}

node_t * add_variable_reference(char * name) {
    variable_node * node = calloc(1, sizeof(variable_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->name = malloc(strlen(name) + 1);
    if (node->name == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->name, name);
    node->declared = FALSE;
    node->type = VARIABLE_NODE;

    return (node_t *)node;
}

node_t * assign_variable_node(char * name, node_t * expression) {
    variable_node * node = calloc(1, sizeof(variable_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->type = VARIABLE_NODE;
    node->name = malloc(strlen(name) + 1);
    if (node->name == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->declared = FALSE;
    node->value = expression;
    strcpy(node->name, name);

    return (node_t *)node;
}

node_t * add_instruction_node(node_t * node) {
    instruction_node * new = malloc(sizeof(instruction_node));
    if (new == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    new->instruction = node;
    new->type = INSTRUCTION_NODE;
    return (node_t *)new;
}

node_t * add_instruction_list_node(node_t * node) {
    node_list * new = malloc(sizeof(node_list));
    if (new == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
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
    } else if (new_node != NULL) {
        new_node->next = current_node;
    }

    return (node_t *)new_node;
}

node_t * add_element_to_block(node_list * list, node_t * element) {
    return (node_t *)add_element_to_list(list, element);
}

node_t * add_if_node(node_t * condition, node_t * then, node_t * otherwise) {
    if_node * new = malloc(sizeof(if_node));
    if (new == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    new->condition = condition;
    new->then = then;
    new->otherwise = otherwise;
    new->type = IF_NODE;
    return (node_t *)new;
}

node_t * add_block_node(node_list * list) {
    block_node * block = malloc(sizeof(block_node));
    if (block == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    block->node_list = (node_t *)list;
    block->type = BLOCK_NODE;

    return (node_t *)block;
}

node_t * add_print_node(node_t * content) {
    print_node * node = malloc(sizeof(print_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->content = content;
    node->type = PRINT_NODE;
    return (node_t *)node;
}

node_t * add_text_node(char * text) {
    text_node * node = (text_node *)malloc(sizeof(text_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->text = malloc(strlen(text) + 1);
    if (node->text == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->text, text);
    node->type = TEXT_NODE;

    return (node_t *)node;
}

node_t * add_number_node(char * number) {
    number_node * node = (number_node *)malloc(sizeof(number_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->number = malloc(strlen(number) + 1);
    if (node->number == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->number, number);
    node->type = NUMBER_NODE;

    return (node_t *)node;
}

node_t * add_list_node(char * array) {
    array_node * node = (array_node *)malloc(sizeof(array_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->array = malloc(strlen(array) + 1);
    if (node->array == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->array, array);
    node->type = ARRAY_NODE;

    return (node_t *)node;
}

node_t * add_operation_node(char * operation) {
    operation_node * node = (operation_node *)malloc(sizeof(operation_node));
    if (node == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    node->operation = malloc(strlen(operation) + 1);
    if (node->operation == NULL) {
        free(node);
        printf("Hubo un error en el maloc");
        return NULL;
    }
    strcpy(node->operation, operation);
    node->type = OPERATION_NODE;

    return (node_t *)node;
}

node_t * add_while_node(node_t * condition, node_t * block) {
    while_node * new = (while_node *)malloc(sizeof(while_node));
    if (new == NULL) {
        printf("Hubo un error en el maloc");
        return NULL;
    }
    new->condition = condition;
    new->then = block;
    new->type = WHILE_NODE;
    return (node_t *)new;
}
