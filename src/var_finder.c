#include "../include/tree.h"
#include "../include/var_finder.h"
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

int check_and_set_variables(node_list * tree){
    check_and_set_variables_internal(tree,NULL);
    return error;
}

void check_and_set_variables_internal(node_list * tree,var_node * var_list){
    node_list * aux = tree;
    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        switch(nodo->instruction->type) {
            case VARIABLE_NODE:
                ;
                variable_node* variable_node_var=(variable_node *)nodo->instruction;
                if (variable_node_var->declared==TRUE){
                    if (check_if_exists(var_list,variable_node_var->name)!=-1)
                    {
                        printf("Var already declared %s\n",variable_node_var->name);
                        error=-1;
                    }
                    var_list=add_to_list(var_list,create_var_node(variable_node_var->type,variable_node_var->name));
                }else{
                    int type =check_if_exists(var_list,variable_node_var->name);
                    if (type==-1)
                    {
                        printf("Var not declared yet %s\n",variable_node_var->name);
                        error=-1;
                    }

                    variable_node_var->type=type;

                }
                break;
            default:
                #ifdef YYDEBUG
                printf("Algo salio mal var checker\n");
                #endif
                break;
        }

       
        aux = aux->next;
    }
    free_list(var_list);
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
