#include "parser/ast_printer.h"


void enqueue(QueueNode **head, QueueNode **tail, const ExprNode *node, int level)
{
  QueueNode *newNode = malloc(sizeof(QueueNode));
  newNode->node = node;
  newNode->level = level;
  newNode->next = NULL;
  if (*tail) (*tail)->next = newNode;
  else *head = newNode;
  *tail = newNode;
}

const ExprNode *dequeue(QueueNode **head, QueueNode **tail, int *level)
{
  if (!*head) return NULL;
  QueueNode *temp = *head;
  const ExprNode *node = temp->node;
  *level = temp->level;
  *head = temp->next;
  if (!*head) *tail = NULL;
  free(temp);
  return node;
}

int tree_height(const ExprNode *node)
{
  if (!node) return 0;
  int lh = tree_height(node->left);
  int rh = tree_height(node->right);
  return (lh > rh ? lh : rh) + 1;
}

void print_node_value(const ExprNode *node)
{
    if (!node) {
        printf(" ");
        return;
    }
    switch (node->type) {
    case NODE_NUMBER:
      if(floor(node->value.number) == node->value.number)
        printf("%.0f", node->value.number);
      else
	printf("%g", node->value.number);
      break;
    case NODE_OPERATION:
        printf("%c", node->value.operator);
        break;
    case NODE_UNARY:
      printf("%c", node->value.operator);
      break;
    case NODE_VARIABLE:
      printf("%s", node->value.name);
      break;
    case NODE_FUNCTION:
        printf("%s", node->value.name);
	break;
    case NODE_ERROR:
      //printf("%s", node->value.error);
      printf("e_node");
        break;
    default:
        printf("?");
    }
}


void print_tree_levels(const ExprNode *root)
{
  if (!root) return;

  QueueNode *head = NULL, *tail = NULL;
  enqueue(&head, &tail, root, 0);

  int current_level = 0;
  printf("Level %d: ", current_level);

  while (head)
    {
      int level;
      const ExprNode *node = dequeue(&head, &tail, &level);

      if (level > current_level)
	{
	  current_level = level;
	  printf("\nLevel %d: ", current_level);
        }

      print_node_value(node);

	  if (node->left)
	    enqueue(&head, &tail, node->left, level + 1);
	  if (node->right)
	    enqueue(&head, &tail, node->right, level + 1);
        }
  printf("\n");
    }

void print_tree_pyramid(const ExprNode *root)
{
  if (!root) return;

  int height = tree_height(root);
  int max_width = (int)pow(2, height) * 2;  // rough width estimate

  QueueNode *head = NULL, *tail = NULL;
  enqueue(&head, &tail, root, 0);

  int current_level = 0;
  int nodes_in_level = 0;
  int expected_in_level = 1;

  printf("\n");

  while (head)
    {
      int level;
      const ExprNode *node = dequeue(&head, &tail, &level);

      // New line for new level
      if (level > current_level)
	{
	  printf("\n\n");
	  current_level = level;
	  nodes_in_level = 0;
	  expected_in_level = (int)pow(2, level);
	}

      // Compute spacing between nodes
      int spacing = max_width / (expected_in_level + 1);

      // Print spaces before each node
      for (int i = 0; i < spacing; i++) printf(" ");

      
      // Print node value
      if (node)
	{
	  print_node_value(node);

	  // enqueue children (including NULL to keep spacing)
	  enqueue(&head, &tail, node->left, level + 1);
	  enqueue(&head, &tail, node->right, level + 1);
	}
      else
	{
	  printf(" ");
	  enqueue(&head, &tail, NULL, level + 1);
	  enqueue(&head, &tail, NULL, level + 1);
	}

      nodes_in_level++;
      if (nodes_in_level == expected_in_level)
	{
	  // level complete
	  if (level >= height - 1) break; // prevent infinite enqueueing of NULLs
	}
    }

  printf("\n\n");
}
