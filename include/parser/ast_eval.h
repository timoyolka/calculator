#ifndef EVAL_TREE_H
#define EVAL_TREE_H

#include "builtins.h"
#include "lexer/token_types.h"
#include "parser/expr_node.h"

//Evaluation
double eval_tree(ExprNode *root);
double eval_number_node(ExprNode *n);
double eval_operation_node(ExprNode *n);
double eval_unary_op_node(ExprNode *n);
double eval_function_node(ExprNode *n);

#endif
