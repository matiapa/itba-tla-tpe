#include "../include/tree.h"
#include "../include/var_finder.h"
#include "../y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_VAR_NAME_LENGTH 256

#define ERROR(...) fprintf(stderr, "\033[38;2;255;0;0mERROR: ");\
    fprintf(stderr, ##__VA_ARGS__);\
    fprintf(stderr, "\x1b[0m\n");\
    ;


int error=0;

typedef struct var_node {
    int var_type;
    char name[MAX_VAR_NAME_LENGTH];
    int references;
    struct var_node * next;
} var_node;


var_node * create_var_node(int type, char * name);
void add_to_list(var_node ** list,var_node * element);
void check_and_set_variables_internal(node_list * tree,var_node ** var_list);
var_node * free_list(var_node * list);
int check_if_exists(var_node * list,char * name);
void check_and_set_variables_rec(node_t * node,var_node ** var_list);
void check_var_types_in_value(int type,variable_node* variable_node_var,var_node * var_list );
int check_var_type_in_expression(int type,expression_node * expr,var_node * var_list);
int check_var_type_in_expression_rec(int type,node_t * node,var_node * var_list);
int check_var_type_in_list_op(int type,list_op_node * node,var_node * var_list);
int check_var_type_in_list_value(int type,list_op_node * node,var_node * var_list);

int check_and_set_variables(node_list * tree){ //inicializa la lista con un pseudo nodo para no pasar null
    var_node varinit;
    varinit.references=2;
    varinit.next=NULL;
    varinit.var_type=-1;
    var_node * var_list=&varinit;

    check_and_set_variables_internal(tree,&var_list);
    return error;
}

void check_and_set_variables_internal(node_list * tree,var_node ** var_list){// recorre las listas de insucciones
    node_list * aux = tree;
    while (aux != NULL) {
        instruction_node * nodo = (instruction_node *)aux->node;
        check_and_set_variables_rec((node_t *)nodo->instruction,var_list);
        aux = aux->next;
    }
    if (var_list!=NULL)
    {
       (*var_list)=free_list(*var_list); 
    }
    
}





void check_and_set_variables_rec(node_t * node,var_node ** var_list){
    switch(node->type) {
            case VARIABLE_NODE:
                ;
                variable_node* variable_node_var=(variable_node *)node;
                if (variable_node_var->declared==TRUE && variable_node_var->value!=NULL){ //caso donde se define la var y asigna
                    if (check_if_exists(*var_list,variable_node_var->name)!=-1)
                    {
                        ERROR("Var %s already declared \n",variable_node_var->name);
                        error=-1;
                    }
                    add_to_list(var_list,create_var_node(variable_node_var->var_type,variable_node_var->name)); //registrar la variable
                    
                    check_var_types_in_value(variable_node_var->var_type,variable_node_var,*var_list); //revisar que la asigncion tiene variables/simbolos validos
                }
                if (variable_node_var->declared==TRUE && variable_node_var->value==NULL){ //caso donde se define la var y no se asigna
                    if (check_if_exists(*var_list,variable_node_var->name)!=-1)
                    {
                        ERROR("Var %s already declared \n",variable_node_var->name);
                        error=-1;
                    }
                    add_to_list(var_list,create_var_node(variable_node_var->var_type,variable_node_var->name)); //registrar la variable
                }
                if (variable_node_var->declared==FALSE && variable_node_var->value!=NULL){ //caso donde no se define la var pero se asigna
                    int type =check_if_exists(*var_list,variable_node_var->name);
                    if (type==LIST_TYPE)
                    {
                        ERROR("Var %s warning, cannot assign variable after declaration\n",variable_node_var->name);
                        error=-1;
                    }
                    
                    if (type==-1)
                    {
                        ERROR("Var %s is not declared yet\n",variable_node_var->name);
                        error=-1;
                    }
                    variable_node_var->var_type=type;
                    check_var_types_in_value(type,variable_node_var,*var_list); //revisar que la asigncion tiene variables/simbolos validos
                }
                if (variable_node_var->declared==FALSE && variable_node_var->value==NULL){ //caso donde solo se usa la var
                    
                    int type =check_if_exists(*var_list,variable_node_var->name); 
                    if (type==-1)
                    {
                        ERROR("Var %s not declared yet \n",variable_node_var->name);
                        error=-1;
                    }
                    variable_node_var->var_type=type;
                }
                
                break;
            case PRINT_NODE: //el caso donde recibis algo como write <algo>
                ;
                print_node * print_node_var = (print_node *) node;
                switch (print_node_var->content->type){
                    case VARIABLE_NODE:
                        check_and_set_variables_rec((node_t *) print_node_var->content,var_list);   
                        break;          
            
                    case EXPRESSION_NODE:
                        if(!check_var_type_in_expression(NUMBER_TYPE,(expression_node *)print_node_var->content,*var_list)){
                            ERROR("Var in write is type text in expression\n");
                            error=-1;
                        }
                        break;
                
                    case TEXT_NODE:
                    //va vacio porque no hay variables aca dentro
                        break;

                    case ARRAY_NODE:
                    //va vacio porque no hay variables aca dentro
                        break;
                    
                    default:
                        #if YYDEBUG == 1
                        printf("Algo salio mal var checker print_node\n");
                        #endif
                        break;
                }
                break;

            case READ_NODE: //el caso donde es read <algo>
                ;
                read_node * read_node_var = (read_node *) node;
                switch (read_node_var->content->type){
                    case VARIABLE_NODE: //read variable
                        check_and_set_variables_rec((node_t *) read_node_var->content,var_list);  
                        if (((variable_node *)read_node_var->content)->var_type!=NUMBER_TYPE)
                        {
                            ERROR("Var %s not of type numeric \n",((variable_node *)read_node_var->content)->name);
                            error=-1;
                        }
                        break;         
                    
                    default:
                        #if YYDEBUG == 1
                        printf("Algo salio mal var checker read_node\n");
                        #endif
                        break;
                }
                break;
            case IF_NODE: 
            /* 
            if condition do
                Block
            else
                Block
            end
            
            */

                ;
                if_node* if_node_var=(if_node *)node;
                if(!check_var_type_in_expression(NUMBER_TYPE,(expression_node *)if_node_var->condition,*var_list)){ // miro que la condicion sea valida
                    ERROR("Var in write is type text in expression\n");
                    error=-1;
                }
                (*var_list)->references++;
                check_and_set_variables_internal( (node_list *)((block_node *)if_node_var->then)->node_list,var_list); //busco variables en el bloque del if
                ;
                // *var_list=free_list(*var_list);
                if (if_node_var->otherwise!=NULL) //else es opcional
                {
                   (*var_list)->references++;
                    check_and_set_variables_internal((node_list *)((block_node *)if_node_var->otherwise)->node_list,var_list); //busco variables en el bloque del else
                    // *var_list=free_list(*var_list);
                }
                
                
                break;
            case WHILE_NODE:
            /* 
            while condition do
                Block
            end
            
            */
                ;
                while_node* while_node_var=(while_node *)node;
                if(!check_var_type_in_expression(NUMBER_TYPE,(expression_node *)while_node_var->condition,*var_list)){ //buscar variables en condicion
                    ERROR("Var in write is type text in expression\n");
                    error=-1;
                }
                (*var_list)->references++;
                check_and_set_variables_internal( (node_list *)((block_node *)while_node_var->then)->node_list,var_list); //buscar variables en el bloque
                //  *var_list=free_list(*var_list);
                break;
            default:
                #if YYDEBUG == 1
                printf("Algo salio mal var checker\n");
                #endif
                break;
        }
}

void check_var_types_in_value(int type,variable_node* variable_node_var,var_node * var_list ){ 

    switch (variable_node_var->value->type){
        case TEXT_NODE:
            if (variable_node_var->var_type!=TEXT_TYPE){
                ERROR("Var %s is not of type text and assigned text\n",variable_node_var->name);
                error=-1;
            }
            break;
        case ARRAY_NODE:
            if (variable_node_var->var_type!=LIST_TYPE){
                ERROR("Var %s is not of type list and assigned list\n",variable_node_var->name);
                error=-1;
            }
            break;
        case EXPRESSION_NODE:
            if (!check_var_type_in_expression(type,(expression_node*)variable_node_var->value,var_list)){
                ERROR("Var %s is of type number and assigned not number\n",variable_node_var->name);    
                error=-1;
            }
            break;
        case VARIABLE_NODE:
            ;
            int type =check_if_exists(var_list,((variable_node *)variable_node_var->value)->name);
            if (type==-1)
            {
                ERROR("Var %s not declared yet \n",((variable_node *)variable_node_var->value)->name);
                error=-1;
            }if (variable_node_var->var_type==0)
            {
                variable_node_var->var_type=type;
            }
            if (type!=variable_node_var->var_type)
            {
               ERROR("Var %s is different type than %s \n",variable_node_var->name,((variable_node *)variable_node_var->value)->name);
               error=-1;
            }
            
            break;
        default:
            #if YYDEBUG == 1
            printf("Algo salio mal var checker types in value\n");
            #endif
            break;
    }
}

int check_var_type_in_expression(int type,expression_node * expr,var_node * var_list){ // expresion tiene 3 espacios que no pueden ser todos nulos, esta llamada rompe la busqueda en cada uno
    if (
        check_var_type_in_expression_rec(type,expr->first,var_list)&& 
        check_var_type_in_expression_rec(type,expr->second,var_list)&&
        check_var_type_in_expression_rec(type,expr->third,var_list))

    {
        return TRUE;
    }
    return FALSE;    
    
}

int check_var_type_in_expression_rec(int type,node_t * node,var_node * var_list){ //resuelve el valor de expresiones
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
                ERROR("Var not declared yet %s\n",variable_node_var->name);
                error=-1;
            }
            variable_node_var->var_type=type_var;
            return type_var==type;
            break;
        case TEXT_NODE:
            return TEXT_TYPE==type;
            break;
        case NUMBER_NODE:
            return NUMBER_TYPE==type;
            break;
        case OPERATION_NODE:
            return NUMBER_TYPE==type;
            break;
        case EXPRESSION_NODE:
            return check_var_type_in_expression(type,(expression_node *)node,var_list);
            break;
        case LIST_OP_NODE:
            ;
            int result=check_var_type_in_list_op(LIST_TYPE,(list_op_node *)node,var_list);
            if (!result)
            {
                ERROR("Var in list operation not of correct type\n");
                error=-1;
            }
            return type==NUMBER_TYPE;
            break;
        default:
            #if YYDEBUG == 1
            printf("Algo salio mal var checker expression rec\n");
            #endif
            break;
    }
    return FALSE; //SHOULD NOT BE HERE
}

int check_var_type_in_list_op(int type,list_op_node * node,var_node * var_list){ //resuelve las funciones de lista
    
    if(strcmp("elem_at",node->operator)==0){
        if (check_var_type_in_list_value(LIST_TYPE,node,var_list)==FALSE){ //caso lista(0)
            ERROR("List value is invalid \n");
            error=-1;
        }
        if (check_var_type_in_expression(NUMBER_TYPE,(expression_node *)node->arg,var_list)==FALSE)
        {
            ERROR("index expression in list is invalid \n");
            error=-1;
        }
        return TRUE;
    }else if(strcmp("contains",node->operator)==0){ 
        if (check_var_type_in_list_value(LIST_TYPE,node,var_list)==FALSE){ //caso n in lista
            ERROR("List is invalid \n");
            error=-1;
        }
        if (check_var_type_in_expression(NUMBER_TYPE,(expression_node *)node->arg,var_list)==FALSE)
        {
            ERROR("expression before IN list is invalid \n");
            error=-1;
        }
        
        return TRUE;
    }else{
        if (check_var_type_in_list_value(LIST_TYPE,node,var_list)==FALSE){ //function of list e.g mean of list
            ERROR("List is invalid \n");
            error=-1;
        }
        
        return TRUE;
    }
}
int check_var_type_in_list_value(int type,list_op_node * node,var_node * var_list){ //revisar dentro del valor de una lista
    switch(node->list->type) {
        case VARIABLE_NODE:
            ;
            variable_node* variable_node_var=(variable_node *)node->list;
            int type_var =check_if_exists(var_list,variable_node_var->name);
            if (type_var==-1)
            {
                ERROR("Var %s in list operation not declared yet \n",variable_node_var->name);
                error=-1;
            }
            variable_node_var->var_type=type_var;
            return type_var==type;
            break;
        
        case ARRAY_NODE:
            //TODO ?
            return TRUE;
            break;
        
        default:
            #if YYDEBUG == 1
            printf("Algo salio mal var checker func call\n");
            #endif
            break;
    }
    return FALSE;
}


var_node * create_var_node(int type, char * name){ //inicializa un nodo de la lista de variables, arranca con 0 referencias para poder ser borrado en caso de ninguna referencia adicional
    var_node * new= malloc(sizeof(var_node));
    new->var_type=type;
    strcpy(new->name,name);
    new->references=0;
    new->next=NULL;
    return new;
}



void add_to_list(var_node ** list,var_node * element){ //pone un nuevo nodo a la cabeza de la lista
    if (*list!=NULL){
        element->next=*list;
    }
    *list=element;

}

var_node * free_list(var_node * list){ //libera nodos de la lista hasta que tengan mas de 1 referencia, en donde deja de liberar y pone el nodo en el comienzo de la lista
    if (list == NULL)
    {
        return NULL;
    }
    
    var_node * current=list;
    while (current!=NULL && current->references<1)
    {
        var_node * next=current->next;
        free(current);
        current=next;
    }
    if (current!=NULL)
    {
        current->references--;
    }
    return current;
}

int check_if_exists(var_node * list,char * name){ //recorre la lista de variables y compara nombres, si no enuentra la variable devuelve -1
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
