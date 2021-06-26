#ifndef _FREE_TREE_H_
#define _FREE_TREE_H_


/*-------------------- FUNCIONES ---------------------*/
void * free_write(node_t * node);
void * free_read(node_t * node);
void * free_variable(node_t * node);
void * free_expression(node_t * node);
void * free_list(node_list * list);
void * free_if_node(node_t * node);
void * free_while_node(node_t * node);
void * free_text_node(node_t * node);
void * free_instruction_list(node_list * list);
void * free_number_node(node_t * node);
void * free_operation_node(node_t * node);
void * free_instruction_node(node_t * node);
void * free_tree(node_list * program);
void * free_function_call(node_t * node);
void * free_array_node(node_t * node);


#endif
