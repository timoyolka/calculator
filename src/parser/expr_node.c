#include "parser/expr_node.h"

ExprNode* create_number_node(double value)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if(!node) { return NULL; }

  node->type = NODE_NUMBER;
  node->value.number = value;
  node->left = node->right = NULL;
  
  return node;
}

ExprNode* create_variable_node(const char *name)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if(!node) { return NULL; }

  node->type = NODE_VARIABLE;
  node->value.name = strdup(name);
  node->left = node->right = NULL;
  
  return node;
}

ExprNode* create_operator_node(char op, ExprNode *left, ExprNode *right)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if(!node) { return NULL; }

  node->type = NODE_OPERATION;
  node->value.operator = op;
  node->left = left;
  node->right = right;
  
  return node;
}

ExprNode* create_function_node(const char *name, ExprNode *arg1, ExprNode *arg2)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if (!node) { return NULL; }
  
  node->type = NODE_FUNCTION;
  node->value.name = strdup(name);  // copy function name
  node->left = arg1 ? arg1 : NULL;
  node->right = arg2 ? arg2 : NULL;                   // functions are unary  
  return node;
}

ExprNode* create_unary_op_node(char op, ExprNode *child)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if(!node) { return NULL; }
  
  node->type = NODE_UNARY;
  node->value.operator = op;
  node->left = NULL;
  node->right = child;

  return node;
}

ExprNode* create_error_node(ExprNode *left, ExprNode *right, const char* err_fmt, ...)
{
  ExprNode *node = malloc(sizeof(ExprNode));
  if (!node) return NULL;

  va_list args;
  va_start(args, err_fmt);

  char *formatted = NULL;
  int len = vsnprintf(NULL, 0, err_fmt, args);
  va_end(args);

  if (len < 0)
    {
      free(node);
      return NULL;
    }

  formatted = malloc(len + 1);
  if (!formatted)
    {
      free(node);
      return NULL;
    }

  va_start(args, err_fmt);
  vsnprintf(formatted, len + 1, err_fmt, args);
  va_end(args);

  node->type = NODE_ERROR;
  node->left = left;
  node->right = right;
  node->value.error = formatted;

  return node;
}
