#ifndef AST_H
#define AST_H

#include <stdlib.h>  // malloc, free
#include <string.h>  // strcpy, strcmp
#include <stdio.h>   // debugging

#include "lexer/lexer_api.h"
#include "parser/expr_node.h"
#include "parser/parser_state.h"
#include "builtins.h"

#define INDENT_STEP 4

//Parses binary + or -
ExprNode* parse_expression(ParserState *p);

//Parses * or /
ExprNode* parse_term(ParserState *p);

//Parses unary + or -
ExprNode* parse_factor(ParserState *p);

//Parses binary ^(power)
ExprNode* parse_power(ParserState *p);

//Parses all other tokens: numbers, variables, function or parenthesses
ExprNode* parse_primary(ParserState *p);

//Free the tree's memory 
void free_expr_tree(ExprNode *root);

#endif /* EXPR_TREE_H */
