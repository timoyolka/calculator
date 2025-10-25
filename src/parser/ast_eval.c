#include "parser/ast_eval.h"

double eval_tree(ExprNode *root)
{
  if(!root) { return 0.0; };
  switch(root->type)
    {
    case NODE_NUMBER:
      return eval_number_node(root);
      break;
    case NODE_VARIABLE:
      return 0.0;
    case NODE_OPERATION:
      return eval_operation_node(root);
      break;
    case NODE_UNARY:
      return eval_unary_op_node(root);
      break;
    case NODE_FUNCTION:
      return eval_function_node(root);
      break;
    default:
      return 0.0;
      break;
    }
}

double eval_number_node(ExprNode *n)
{
  return n->value.number;
}

double eval_operation_node(ExprNode *n)
{
  switch(n->value.operator)
    {
    case ADD:
      return eval_tree(n->left) + eval_tree(n->right);
      break;
    case SUB:
      return eval_tree(n->left) - eval_tree(n->right);
      break;
    case MULT:
      return eval_tree(n->left) * eval_tree(n->right);
      break;
    case DIV:
      return eval_tree(n->left) / eval_tree(n->right);
      break;
    case POW:
      return pow(eval_tree(n->left), eval_tree(n->right));
      break;
    default:
      return 0.0;
    break;
    }
}

double eval_unary_op_node(ExprNode *n)
{
  switch(n->value.operator)
    {
    case ADD:
      return eval_tree(n->right);
      break;
    case SUB:
      return -eval_tree(n->right);
      break;
    default:
      fprintf(stderr, "Evaluation error: unknown unary operator '%c'\n", n->value.operator);
      return NAN;
      break;			   
    }
}

double eval_function_node(ExprNode *n)
{
  MathFuncN func = get_function(n->value.name);
  if (!func)
    {
      fprintf(stderr, "Unknown function: %s\n", n->value.name);
      return 0.0;
    }
  int argc = 0;
  double argv[2];
  if(n->left) argv[argc++] = eval_tree(n->left);
  if(n->right) argv[argc++] = eval_tree(n->right);
  
  return func(argv, argc);
}
