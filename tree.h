#ifndef _TREE_H_
#define _TREE_H_

#define TRUE 1
#define FALSE 0

typedef enum {
  STRING_NODE = 0,
  CONSTANT_NODE,
  VARIABLE_NODE,
  OPERATION_NODE,
  CONDITIONAL_NODE,
  BLOCK_NODE,
  EMPTY_NODE,
  IF_NODE,
  WHILE_NODE,
  RETURN_NODE,
  LIST_NODE,
  INSTRUCTION_NODE,
  NEGATION_NODE,
  PRINT_NODE,
  EXPRESSION_NODE
} node_type;

typedef struct node_s {
    node_type type;
} node_t;

typedef struct node_list{
  node_type type;
  node_t * node;
  struct node_list *next;
} node_list;

typedef struct variable_node {
    node_type type;
    int declared;
    char * var_type;
    char * name;
    node_t * value;
} variable_node;

typedef struct expression_node {
    node_type type;
    char * expression;
} expression_node;

typedef struct instruction_node {
    node_type type;
    node_t * instruction;
} instruction_node;


typedef struct block {
    node_type type;

} block;

typedef struct if_node {
    node_type type;
    node_t * condition;
    node_t * then;
    node_t * otherwise;
} if_node;

node_t * declare_variable_node(char * name, char * var_type);
node_t * add_value_variable(node_t * var_node, node_t * expression);
node_t * assign_variable_node(char * name, node_t * expression);

node_t * add_expression(char * expression);
node_t * add_instruction_node(node_t * node);
node_t * add_instruction_list_node(node_t * node);
node_t * add_element_to_list(node_list * list, node_t * element);

#endif

