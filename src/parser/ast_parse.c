#include "parser/ast_parse.h"

ExprNode* parse_function_call(ParserState *p, const char *fname)
{
  CalcToken *tok = current(p);

  if (!tok || tok->type != TOKEN_LPAREN) {
    return create_error_node(NULL, NULL,
			     "Syntax error: expected '(' after function '%s' at token number: %zu\n",
			     fname, p->count);
  }

  advance_parser(p); // consume '('
  ExprNode *arg1 = parse_expression(p);

  if (!arg1) {
    return create_error_node(NULL, NULL,
			     "Syntax error: invalid expression in function '%s' argument at token %zu\n",
			     fname, p->count);
  }

  // Optional second argument
  tok = current(p);
  ExprNode *arg2 = NULL;

  if (tok && tok->type == TOKEN_COMMA) {
    advance_parser(p);
    arg2 = parse_expression(p);
    if (!arg2){
      return create_error_node(NULL, NULL,
			       "Invalid second argument in function '%s'", fname);
    }
  }

  tok = current(p);
  if (!tok || tok->type != TOKEN_RPAREN) {
    return create_error_node(NULL, NULL,
			     "Syntax error: expected ')' after function '%s' arguments at token number: %zu\n",
			     fname, p->count);
  }

  advance_parser(p);
  return create_function_node(fname, arg1, arg2);
}

ExprNode* parse_parenthesized_expression(ParserState *p)
{
  ExprNode *node = parse_expression(p);
  CalcToken *tok = current(p);

  if (!tok || tok->type != TOKEN_RPAREN) {
    return create_error_node(NULL, NULL,
			     "Syntax error: expected ')' at token number: %zu\n", p->count);
  }

  advance_parser(p);
  return node;
}

ExprNode* parse_expression(ParserState *p)
{
  //Parse the leftmost token, consumes the token
  ExprNode *left = parse_term(p);
  CalcToken *tok = current(p);
  if(!tok) { return NULL; }

  //While the next token is + or -
  while(tok &&
	tok->type != TOKEN_END &&
	tok->type == TOKEN_OPERATOR &&
	(*tok->value.text == ADD || *tok->value.text == SUB)) {
      //Remember the operator(+ or -)
      char op = *tok->value.text;
      advance_parser(p);

      //Parse the next token(right side to this operator)
      ExprNode *right = parse_term(p);
      if (!right) {
	  return create_error_node(left, NULL,
				   "Syntax error: expected term after '%c' at token number: %zu\n",
				   op, p->count);
        }

      //Set left to this node so next loop iteration extends it
      left = create_operator_node(op, left, right);
      
      tok = current(p);
    }
    
  
  //When no more + or - operators are present, return subtree built so far
  return left;
}

ExprNode* parse_term(ParserState *p)
{
  // Parse the leftmost factor
  ExprNode *left = parse_factor(p);
  if (!left) return NULL;

  CalcToken *tok = current(p);

  while (tok && tok->type != TOKEN_END) {
      //Explicit multiplication or division
      if (tok->type == TOKEN_OPERATOR &&
	  (*tok->value.text == MULT || *tok->value.text == DIV)) {
	  char op = *tok->value.text;
	  advance_parser(p);

	  ExprNode *right = parse_factor(p);
	  if (!right) {
	      return create_error_node(left, NULL,
				       "Syntax error: expected term after '%c' at token number: %zu\n",
				       op, p->count);
            }
	  

	  left = create_operator_node(op, left, right);
        }
      //Implicit multiplication
      else if (tok->type == TOKEN_NUMBER ||
	       tok->type == TOKEN_VARIABLE ||
	       tok->type == TOKEN_FUNCTION ||
	       tok->type == TOKEN_LPAREN) {
	  ExprNode *right = parse_factor(p);
	  if (!right) {
	      return create_error_node(left, NULL,
				       "Syntax error: expected term after implicit '*'");
            }
	  left = create_operator_node(MULT, left, right);
        } else {
	  break;
        }

      tok = current(p);
    }

  return left;
}


ExprNode* parse_factor(ParserState *p)
{
  CalcToken *tok = current(p);

  //Check weather the current token is a + or -
  if(tok &&
     tok->type != TOKEN_END &&
     tok->type == TOKEN_OPERATOR &&
     (*tok->value.text == ADD || *tok->value.text == SUB)) {
      //Rember the operator and consume the token
      char op = *tok->value.text;
      advance_parser(p);

      //Since this function handle unary operation, needs only one child, unary operation are right-associative
      ExprNode *child = parse_factor(p);
      if (!child) {
	  return create_error_node(NULL, NULL,
				   "Syntax error: expected term after '%c' at token number: %zu\n",
				   op, p->count);
	}
      
      return create_unary_op_node(op, child);
    }
  //Move to 
  return parse_power(p);
}


ExprNode* parse_power(ParserState *p)
{
  ExprNode *left = parse_primary(p);
  if (!left) { return NULL; }

  CalcToken *tok = current(p);
  if(!tok) { return NULL; }

  //Check weather the current token is a ^
  if(tok->type != TOKEN_END &&
     tok->type == TOKEN_OPERATOR &&
     *tok->value.text == POW) {
      //Consume the token
      advance_parser(p);

      //Power is a binary operator, so create the subtree for chain powers -? x^x^x^...
      ExprNode *right = parse_power(p);

      if (!right) {	 
	  return create_error_node(NULL, NULL,
				   "Syntax error: expected right-hand side after '^' at token number: %zu\n",
				   p->count);
	}
      
      left = create_operator_node(POW, left, right);
    }
  return left;
}

ExprNode* parse_primary(ParserState *p)
{
  CalcToken *tok = current(p);
  if (!tok || tok->type == TOKEN_END) {
    return NULL;
  }

  switch (tok->type) {
  case TOKEN_NUMBER: {
    advance_parser(p);
    return create_number_node(tok->value.number);
  }

  case TOKEN_VARIABLE: {
    advance_parser(p);
    return create_variable_node(tok->value.text);
  }

  case TOKEN_FUNCTION: {
    char *fname = tok->value.text;
    advance_parser(p);
    return parse_function_call(p, fname);
  }

  case TOKEN_LPAREN: {
    advance_parser(p);
    return parse_parenthesized_expression(p);
  }

  default: {
    return create_error_node(NULL, NULL,
			     "Syntax error: unexpected token in parse_primary at token number: %zu\n",
			     p->count);
  }
  }
}



void free_expr_tree(ExprNode *root)
{
  if(root == NULL) { return; }

  free_expr_tree(root->right);
  free_expr_tree(root->left);

  if(root->type == NODE_FUNCTION && root->value.name != NULL)
    {
      free(root->value.name);
    }
  else if (root->type == NODE_ERROR && root->value.error)
    {
      free(root->value.error);
    }
  free(root);
}
