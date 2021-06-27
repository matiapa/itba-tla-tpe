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
void print_read(node_t * node);
void print_var(node_t * node);
void print_expression(node_t * node);
void read_instruction_list(node_list * list);
void print_if_node(node_t * node);
void print_while_node(node_t * node);
void print_list_op(node_t * node);
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
            case READ_NODE:
                print_read(nodo->instruction);
                break;
            case IF_NODE:
                print_if_node(nodo->instruction);
                break;
            case WHILE_NODE:
                print_while_node(nodo->instruction);
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
        // Es una declaracion
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
    } else {
        // Es una asignacion/referenciacion
        P("%s", (var->name));
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
            array->array[strlen(array->array)-1] = 0;
            P("{%s}", array->array+1);
            free(array->array);
        } else if (var->value->type == VARIABLE_NODE) {
            variable_node * var_node = (variable_node *) var->value;
            P("%s", var_node->name);
            free(var_node->name);
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
                P("printf(\"%%lf\\n\", (double) (%s));\n", var->name);
            if(var->var_type == TEXT_TYPE)
                P("printf(\"%%s\\n\", %s);\n", var->name);
            if (var->var_type == LIST_TYPE) {
                P("print_array(%s, sizeof(%s)/sizeof(double), 0.0);\n", var->name, var->name);
            }
            free(var->name);
            break;
        case EXPRESSION_NODE:
            P("printf(\"%%lf\\n\", (double) (");
            print_expression(print->content);
            P("));\n");
            break;
        case TEXT_NODE:
            ;
            text_node * text = (text_node *)print->content;
            P("printf(\"%%s\\n\", %s);\n", text->text);
            free(text->text);
            break;
        case ARRAY_NODE:
            ;
            array_node * array = (array_node *)print->content;
            P("str_caller(\"%s\", 0.0, %s);\n", array->array, "print_array");
            free(array->array);
        default:
            break;
    }
    free(print->content);
}


void print_read(node_t * node) {
    read_node * read = (read_node *) node;
    
    switch(read->content->type) {
        case VARIABLE_NODE:
            ;
            variable_node * var = (variable_node *)(read->content);
            if (var->var_type == NUMBER_TYPE)
                P("scanf(\"%%lf\", &%s);",var->name); 
            free(var->name);
            break;
        default:
            break;
    }
    free(read->content);
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
        case LIST_OP_NODE:
            print_list_op(node);
            break;
        default:
            break;
    }
}

int print_binary_operation(node_t * first, operation_node * second, node_t * third) {

    if (strcmp(second->operation, "%") == 0) {
        P("(int)");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        if (second != NULL) {
            switch_print_expression((node_t *)second);
            free(second);
        }
        P("(int)");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        return 1;
    } else if (strcmp(second->operation, "^") == 0) {
        P(" pow(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "E ") == 0) {
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(" * ");
        P(" pow(10,");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "comb") == 0) {
        P("combination(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "perm") == 0) {
        P("permutation(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "!") == 0) {
        P("factorial(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(") ");
        return 1;
    }

    return 0;

}

int print_unary_operation(operation_node * first, node_t * second, node_t * third) {
    
    return 0;
}

void print_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;
    int op = 0;

    if (node->first != NULL && node->first->type == OPERATION_NODE) {
        op = print_unary_operation((operation_node *)node->first, node->second, node->third);
    } 

    if (node->second != NULL && node->second->type == OPERATION_NODE) {
        op = print_binary_operation(node->first, (operation_node *)node->second, node->third);
    } 
    
    if (!op) {

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

void print_list_op(node_t * node) {
    list_op_node * lop = (list_op_node *)node;

    if (lop->list->type == ARRAY_NODE) {
        char * str = ((array_node *) lop->list)->array;
        P("str_caller(\"%s\", %lf, %s)", str, lop->arg, lop->operator);
        free(str);
    } else if (lop->list->type == VARIABLE_NODE) {
        variable_node * symbol = (variable_node *) lop->list;
        P("%s(%s, sizeof(%s)/sizeof(double), %lf)", lop->operator, symbol->name, symbol->name, lop->arg);
        free(symbol->name);
    }

    free(lop->list);
    free(lop->operator);
}
