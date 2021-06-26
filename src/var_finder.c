#include "../include/tree.h"
#include "../include/var_finder.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_VAR_NAME_LENGTH 256


int error=0;

typedef struct var_node {
    int var_type;
    char name[MAX_VAR_NAME_LENGTH];
    int references;
    struct var_node * next;
} var_node;


var_node * create_var_node(int type, char * name);
var_node * add_to_list(var_node * list,var_node * element);
void check_and_set_variables_internal(node_list * tree,var_node * var_list);
void free_list(var_node * list);
int check_if_exists(var_node * list,char * name);
var_node * check_and_set_variables_rec(node_t * node,var_node * var_list);
void check_var_types_in_value(variable_node* variable_node_var,var_node * var_list );
int check_var_type_in_expression(int type,expression_node * expr,var_node * var_list);
int check_var_type_in_expression_rec(int type,node_t * node,var_node * var_list);

int check_and_set_variables(node_list * tree){
    check_and_set_variables_internal(tree,NULL);
    return error;
}

void check_and_set_variables_internal(node_list * tree,var_node * var_list){
    node_list * aux = tree;
    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        var_list=check_and_set_variables_rec((node_t *)nodo->instruction,var_list);
        aux = aux->next;
    }
    free_list(var_list);  //todo mejorar free
    
}





var_node * check_and_set_variables_rec(node_t * node,var_node * var_list){
    switch(node->type) {
            case VARIABLE_NODE:
                ;
                variable_node* variable_node_var=(variable_node *)node;
                if (variable_node_var->declared==TRUE && variable_node_var->value!=NULL){ //caso donde se define la var y asigna
                    if (check_if_exists(var_list,variable_node_var->name)!=-1)
                    {
                        printf("Var %s already declared \n",variable_node_var->name);
                        error=-1;
                    }
                    var_list=add_to_list(var_list,create_var_node(variable_node_var->var_type,variable_node_var->name));
                    
                    check_var_types_in_value(variable_node_var,var_list);
                }
                if (variable_node_var->declared==TRUE && variable_node_var->value==NULL){ //caso donde se define la var y no se asigna
                    if (check_if_exists(var_list,variable_node_var->name)!=-1)
                    {
                        printf("Var %s already declared \n",variable_node_var->name);
                        error=-1;
                    }
                    var_list=add_to_list(var_list,create_var_node(variable_node_var->type,variable_node_var->name));
                }
                if (variable_node_var->declared==FALSE && variable_node_var->value!=NULL){ //caso donde se define la var
                    int type =check_if_exists(var_list,variable_node_var->name);
                        if (type==-1)
                        {
                            printf("Var %s not declared yet \n",variable_node_var->name);
                            error=-1;
                        }
                        variable_node_var->type=type;
                        check_var_types_in_value(variable_node_var,var_list);
                }
                if (variable_node_var->declared==FALSE && variable_node_var->value==NULL){ //caso donde solo se usa la var
                    
                    int type =check_if_exists(var_list,variable_node_var->name);
                    if (type==-1)
                    {
                        printf("Var %s not declared yet \n",variable_node_var->name);
                        error=-1;
                    }
                }
                
                break;
            case PRINT_NODE:
                ;
                print_node * print_node_var = (print_node *) node;
                switch (print_node_var->content->type)
                {
                case VARIABLE_NODE:
                    check_and_set_variables_rec((node_t *) print_node_var->content,var_list);   
                    break;
                
                default:
                    #ifdef YYDEBUG
                    printf("Algo salio mal var checker print_node\n");
                    #endif
                    break;
                }
                break;
            default:
                #ifdef YYDEBUG
                printf("Algo salio mal var checker\n");
                #endif
                break;
        }
        
        return var_list;
}

void check_var_types_in_value(variable_node* variable_node_var,var_node * var_list ){

    switch (variable_node_var->value->type){
        case TEXT_NODE:
            if (variable_node_var->var_type!=TEXT_TYPE){
                printf("Var %s is of type text and assigned not text\n",variable_node_var->name);
                error=-1;
            }
            break;
        case EXPRESSION_NODE:
            if (!check_var_type_in_expression(NUMBER_TYPE,(expression_node*)variable_node_var->value,var_list)){
                printf("Var %s is of type number and assigned not number\n",variable_node_var->name);
                error=-1;
            }
            break;
        default:
            #ifdef YYDEBUG
            printf("Algo salio mal var checker print_node\n");
            #endif
            break;
    }
}

int check_var_type_in_expression(int type,expression_node * expr,var_node * var_list){
    if (
        check_var_type_in_expression_rec(type,expr->first,var_list)&&
        check_var_type_in_expression_rec(type,expr->second,var_list)&&
        check_var_type_in_expression_rec(type,expr->third,var_list))

    {
        return TRUE;
    }
    return FALSE;    
    
}

int check_var_type_in_expression_rec(int type,node_t * node,var_node * var_list){
    if (node == NULL)
    {
        return TRUE;
    }
    switch(node->type) {
        case VARIABLE_NODE:
            ;
            variable_node* variable_node_var=(variable_node *)node;
            int type_var =check_if_exists(var_list,variable_node_var->name);
            if (type_var==-1)
            {
                printf("Var not declared yet %s\n",variable_node_var->name);
                error=-1;
            }
            variable_node_var->var_type=type_var;
            return type_var==type;
            break;
        case TEXT_NODE:
        case NUMBER_NODE:
        case OPERATION_NODE:
            return TRUE;
            break;
        case EXPRESSION_NODE:
            return check_var_type_in_expression(type,(expression_node *)node,var_list);
            break;
        default:
            #ifdef YYDEBUG
            printf("Algo salio mal var checker expression rec\n");
            #endif
            break;
    }
    return FALSE; //SHOULD NOT BE HERE
}



var_node * create_var_node(int type, char * name){
    var_node * new= malloc(sizeof(var_node));
    new->var_type=type;
    strcpy(new->name,name);
    new->references=0;
    new->next=NULL;
    return new;
}



var_node * add_to_list(var_node * list,var_node * element){
    if (list==NULL)
    {
        return element;
    }
    list->references++;
    element->next=list;
    return element;
}

void free_list(var_node * list){
    if (list == NULL)
    {
        return;
    }
    
    var_node * current=list;
    while (current!=NULL && current->references<=1)
    {
        var_node * next=current->next;
        free(current);
        current=next;
    }
    if (current!=NULL)
    {
        current->references--;
    }
}

int check_if_exists(var_node * list,char * name){
    var_node * current=list;
    while (current!=NULL)
    {
        var_node * next=current->next;
        if (strcmp(current->name,name)==0)
        {
            return current->var_type;
        }       
        current=next;
    }
    return -1;
}
