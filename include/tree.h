#ifndef _TREE_H_
#define _TREE_H_

#define TRUE 1
#define FALSE 0

enum { EXPRESSION };

typedef enum {
  TEXT_NODE = 0,
  NUMBER_NODE,
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
  EXPRESSION_NODE,
  ARRAY_NODE
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
    int var_type;
    char * name;
    node_t * value;
} variable_node;

// los nodos de expression pueden ser 
// TEXT_NODE, VARIABLE_NODE, NUMBER_NODE
typedef struct expression_node {
    node_type type;
    node_t * first;
    node_t * second;
    node_t * third;
    int cant;
} expression_node;
// los nodos de expression pueden ser 
// TEXT_NODE, VARIABLE_NODE, NUMBER_NODE
// o EXPRESSION_NODE

typedef struct instruction_node {
    node_type type;
    node_t * instruction;
} instruction_node;


typedef struct block_node {
    node_type type;
    node_t * node_list;
} block_node;

typedef struct if_node {
    node_type type;
    node_t * condition; // esto es un expression_node
    node_t * then; // esto es un block_node
    node_t * otherwise; // esto es un block_node
} if_node;

typedef struct print_node {
    node_type type;
    node_t * content; // esto puede ser una variable_node o algo de tipo expression_node
} print_node;

typedef struct text_node {
    node_type type;
    char * text;
} text_node;

typedef struct number_node {
    node_type type;
    char * number;
} number_node;

typedef struct array_node {
    node_type type;
    char * array;
} array_node;

typedef struct operation_node {
    node_type type;
    char * operation;
} operation_node;

node_t * declare_variable_node(char * name, int var_type);
node_t * add_value_variable(node_t * var_node, node_t * expression);
node_t * assign_variable_node(char * name, node_t * expression);
node_t * add_variable_reference(char * name);

node_t * add_expression_node(node_t * first, node_t * second, node_t * third, int cant);
node_t * add_instruction_node(node_t * node);
node_t * add_instruction_list_node(node_t * node);
node_t * add_element_to_list(node_list * list, node_t * element);

node_t * add_print_node(node_t * content);
node_t * add_text_node(char * text);
node_t * add_list_node(char * array);
node_t * add_number_node(char * number);
node_t * add_if_block(node_list * list);
node_t * add_if_node(node_t * condition, node_t * then, node_t * otherwise);

node_t * add_operation_node(char * operation);

#endif

