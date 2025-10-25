#ifndef EXPR_NODE_STRUCT_H
#define EXPR_NODE_STRUCT_H

#include <stdio.h>   // printf, srpintf, vsnprintf
#include <stdlib.h>  // malloc, free
#include <string.h>  // strcpy, strcmp
#include <stdarg.h>  //for argument list

typedef enum {
  NODE_NUMBER,
  NODE_VARIABLE,
  NODE_OPERATION,
  NODE_UNARY,
  NODE_FUNCTION,
  NODE_ERROR
} NodeType;

typedef struct ExprNode{
  NodeType type;
  union {
    double number;
    char operator;
    char *name;
    char *error;
  }value;
  
  struct ExprNode *left;
  struct ExprNode *right;
}ExprNode;


ExprNode* create_number_node(double value);
ExprNode* create_variable_node(const char *name);
ExprNode* create_operator_node(char op, ExprNode *left, ExprNode *right);
ExprNode* create_unary_op_node(char op, ExprNode *child);
ExprNode* create_function_node(const char *name, ExprNode *arg);
ExprNode* create_error_node(ExprNode *left, ExprNode *right, const char *err, ...);


#endif
