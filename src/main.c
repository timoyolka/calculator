#include "lexer/lexer_api.h"
#include "parser/ast_parse.h"
#include "parser/ast_eval.h"
#include "parser/ast_printer.h"


/* 
  General notes:
  1. your synax convetions are weird, they are not consistent which makes it hard to follow and read the code.
  2.  
*/
int main(int argc, char *argv[])
{ 
  // It would be better if you used an enum
  // and argc != 2 with `printf("Calculator.exe <Expresion>")`
  if(argc < 2)
    {
      printf("No argument was given!\n");
      return 1;
    }
  else if(argc > 2)
    {
      printf("Too many arguments!\n");
      return 1;
    }
  
  printf("NOTE: math expression should be passed with double quotes.\n");
  char *math_expression = argv[1];
  TokenNode *head = lex_expr(math_expression);

  print_tokens(head);

  ParserState *p = malloc(sizeof(ParserState));
  init_parser(p, head);


  ExprNode *root = parse_expression(p);

  if(!root)
    {
      printf("Parsing failed or empty expression\n");
    }
  else
    {
      printf("Expression tree:\n");
      print_tree_pyramid(root);
    }
  
  double result = eval_tree(root);
  printf("%s = %f\n", math_expression, result);
  
  free_expr_tree(root);
}
