#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <math.h>

#include "parser/expr_node.h"

typedef struct QueueNode
{
  const ExprNode *node;
  int level;
  struct QueueNode *next;
} QueueNode;

void enqueue(QueueNode **head, QueueNode **tail, const ExprNode *node, int level);
const ExprNode *dequeue(QueueNode **head, QueueNode **tail, int *level);
int tree_height(const ExprNode *node);
void print_node_value(const ExprNode *node);
void print_tree_levels(const ExprNode *root);
void print_tree_pyramid(const ExprNode *root);

#endif
