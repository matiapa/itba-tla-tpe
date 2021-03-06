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
void print_list_mutation_node(node_t * node);
void free_text_node(node_t * node);
void free_number_node(node_t * node);
void free_operation_node(node_t * node);

void read_tree(node_list * program, FILE * file) {
    
    #if YYDEBUG == 1
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
            case LIST_MUTATION_NODE:
                print_list_mutation_node(nodo->instruction);
            default:
                #if YYDEBUG == 1
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
        // Es una declaración
        switch(var->var_type) {
            case NUMBER_TYPE:
                P("double %s", var->name);
                break;
            case TEXT_TYPE:
                P("char * %s", var->name);
                break;
            case LIST_TYPE:
                if (var->value->type == VARIABLE_NODE) {
                    P("double * %s", var->name);
                } else if (var->value->type == NUMBER_NODE) {
                    P("double %s[%s]", var->name, ((number_node *) var->value)->number);
                } else if (var->value->type == ARRAY_NODE) {
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
        // Es una asignacion/referenciación
        P("%s", (var->name));
    }
    free(var->name);

    // si la variable esta asignada
    if (var->value != NULL) {
        P(" = ");
        if (var->var_type == LIST_TYPE && var->value->type == NUMBER_NODE) {
            number_node * num_node = (number_node *) var->value;
            P("{0}");
            free(num_node->number);
        } else if (var->value->type == EXPRESSION_NODE) {
            print_expression(var->value);
        } else if (var->value->type == TEXT_NODE) {
            text_node * text = (text_node *) var->value;
            P("%s", text->text);
            free(text->text);
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
    // dependiendo del tipo de contenido del print se corre una función distinta
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
    // solo se puede leer tipos númericos
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
    
    if (strcmp(second->operation, "%") == 0) { // si la operación es de modulo
        P("(int) (");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(")");
        if (second != NULL) {
            switch_print_expression((node_t *)second);
            free(second);
        }
        P("(int) (");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(")");
        return 1;
    } else if (strcmp(second->operation, "^") == 0) { // si la operación es de potencia
        P(" pow(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        free_operation_node((node_t *)second);
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "E ") == 0) { // si la operación es potencia de 10
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        free_operation_node((node_t *)second);
        P(" * ");
        P(" pow(10,");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "comb") == 0) { // si la operación es de combinatoria
        P("combination(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        free_operation_node((node_t *)second);
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "perm") == 0) { // si la operación es de permutación
        P("permutation(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        free_operation_node((node_t *)second);
        P(", ");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(") ");
        return 1;
    } else if (strcmp(second->operation, "!") == 0) { // si la operación es de factorial
        P("factorial(");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(") ");
        free_operation_node((node_t *)second);
        return 1;
    } else if (strcmp(second->operation, "==") == 0) { // si la operación es de igualdad
        P("(absd((");
        if (first != NULL) {
            switch_print_expression(first);
            free(first);
        }
        P(") - (");
        if (third != NULL) {
            switch_print_expression(third);
            free(third);
        }
        P(")) <= 0.000001)");
        free_operation_node((node_t *)second);
        return 1;
    }

    return 0;

}

void print_expression(node_t * exp) {
    expression_node * node = (expression_node *)exp;
    int op = 0;

    // si la expression es una operación binaria
    if (node->second != NULL && node->second->type == OPERATION_NODE) {
        // si la operación es binaria y requiere una traducción especial
        op = print_binary_operation(node->first, (operation_node *)node->second, node->third);
    } 
    
    // caso contrario
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
    // se imprime la expression
    print_expression(aux->condition);
    free(aux->condition);
    P(") {\n");
    block_node * block = (block_node *)aux->then;
    // se imprime la lista de instrucciones dentro del bloque del then del if
    read_instruction_list((node_list *)block->node_list);
    free(block);
    // si tiene un else
    if (aux->otherwise != NULL) {
        P("\n} else {\n");
        block = (block_node *)aux->otherwise;
        // se imprime la lista de instrucciones dentro del bloque del otherwise del if
        read_instruction_list((node_list *)block->node_list);
        free(block);
    }
    P("}\n");
}

void print_while_node(node_t * node) {
    while_node * aux = (while_node *)node;
    P("while (");
    // se imprime la expression
    print_expression(aux->condition);
    free(aux->condition);
    P(") {\n");
    block_node * block = (block_node *)aux->then;
    // se imprime la lista de instrucciones dentro del bloque del then del while
    read_instruction_list((node_list *)block->node_list);
    free(block);
    P("}\n");
}

void print_list_op(node_t * node) {
    list_op_node * lop = (list_op_node *)node;

    if (lop->list->type == ARRAY_NODE) {
        char * str = ((array_node *) lop->list)->array;
        // se imprime la llamada a función de la operación que se pidio
        P("str_caller(\"%s\", (", str);
        print_expression(lop->arg);
        P("), %s)", lop->operator);
        free(str);
    } else if (lop->list->type == VARIABLE_NODE) {
        variable_node * symbol = (variable_node *) lop->list;
        P("%s(%s, sizeof(%s)/sizeof(double), (", lop->operator, symbol->name, symbol->name);
        print_expression(lop->arg);
        P("))");
        free(symbol->name);
    }

    free(lop->list);
    free(lop->operator);
    free(lop->arg);
}

void print_list_mutation_node(node_t * node) {
    list_mutation_node * lm = (list_mutation_node *) node;

    P("%s[(int) (", lm->list_name);
    print_expression(lm->pos_exp);
    P(")] = ")
    print_expression(lm->value_exp);
    P(";\n")

    free(lm->list_name);
    free(lm->pos_exp);
    free(lm->value_exp);
}
