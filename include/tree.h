#ifndef _TREE_H_
#define _TREE_H_

#define TRUE 1
#define FALSE 0

typedef enum {
  TEXT_NODE = 0,
  NUMBER_NODE,
  ARRAY_NODE,
  OPERATION_NODE,
  VARIABLE_NODE,
  PRINT_NODE,
  IF_NODE,
  WHILE_NODE,
  BLOCK_NODE,
  LIST_NODE,
  INSTRUCTION_NODE,
  NEGATION_NODE,
  EXPRESSION_NODE,
  FUNCTION_CALL_NODE
} node_type;

typedef struct node_s {
    node_type type;
} node_t;

typedef struct node_list{
  node_type type;
  node_t * node; // INSTRUCTION_NODE
  struct node_list * next; // LIST_NODE
} node_list;

typedef struct variable_node {
    node_type type;
    int declared;
    int var_type;
    char * name;
    node_t * value; // EXPRESSION_NODE TEXT_NODE NUMBER_NODE ARRAY_NODE
} variable_node;

// los nodos de expression pueden ser 
// TEXT_NODE, VARIABLE_NODE, NUMBER_NODE, EXPRESSION_NODE
typedef struct expression_node {
    node_type type;
    node_t * first; // EXPRESSION_NODE NUMBER_NODE VARIABLE_NODE OPERATION_NODE
    node_t * second; // EXPRESSION_NODE NUMBER_NODE VARIABLE_NODE OPERATION_NODE
    node_t * third; // EXPRESSION_NODE NUMBER_NODE VARIABLE_NODE OPERATION_NODE
} expression_node;

typedef struct instruction_node {
    node_type type;
    node_t * instruction; // INSTRUCTION_NODE
} instruction_node;

typedef struct function_call_node {
    node_type type;
    char * function_name;
    node_t * input_list;    // ARRAY_NODE or VARIABLE_NODE
} function_call_node;

typedef struct block_node {
    node_type type;
    node_t * node_list; // LIST_NODE
} block_node;

typedef struct if_node {
    node_type type;
    node_t * condition; // EXPRESSION_NODE
    node_t * then; // BLOCK_NODE
    node_t * otherwise; // BLOCK_NODE
} if_node;

typedef struct while_node {
    node_type type;
    node_t * condition; // EXPRESSION_NODE
    node_t * then; // BLOCK_NODE
} while_node;

typedef struct print_node {
    node_type type;
    node_t * content; // EXPRESSION_NODE o VARIABLE_NODE
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

node_t * add_expression_node(node_t * first, node_t * second, node_t * third);
node_t * add_instruction_node(node_t * node);
node_t * add_instruction_list_node(node_t * node);
node_t * add_element_to_list(node_list * list, node_t * element);

node_t * add_print_node(node_t * content);
node_t * add_text_node(char * text);
node_t * add_list_node(char * array);
node_t * add_number_node(char * number);
node_t * add_block_node(node_list * list);
node_t * add_if_node(node_t * condition, node_t * then, node_t * otherwise);
node_t * add_while_node(node_t * condition, node_t * block);

node_t * add_operation_node(char * operation);

node_t * add_function_call(char * function_name, node_t * input);

#endif

