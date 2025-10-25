#include "parser/ast_parse.h"

ExprNode* parse_expression(ParserState *p)
{
  //Parse the leftmost token, consumes the token
  ExprNode *left = parse_term(p);
  Token *tok = current(p);
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

  Token *tok = current(p);

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
  Token *tok = current(p);

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

  Token *tok = current(p);
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
  Token *tok = current(p);
  if(!tok) { return NULL; }
  if(tok->type == TOKEN_END) { return NULL; }

  //Number
  if(tok->type == TOKEN_NUMBER) {
      advance_parser(p);
      return create_number_node(tok->value.number);
    }

  //variable
  if(tok->type == TOKEN_VARIABLE) {
      advance_parser(p);
      return create_variable_node(tok->value.text);
    }

  //Function
  if(tok->type == TOKEN_FUNCTION) {
      //Remember the function name
      char *fname = tok->value.text;

      //Move to '('
      advance_parser(p);
      tok = current(p);

      if(!tok || tok->type != TOKEN_LPAREN) {
	  return create_error_node(NULL, NULL,
				   "Syntax error: expected '(' after function '%s' at token number: %zu\n",
				   fname, p->count);
	}

      //Consume '('
      advance_parser(p);

      //Build a subtree for the argument expression of the function
      ExprNode *arg1 = parse_expression(p);
      if(!arg1) {
	  return create_error_node(NULL, NULL,
			   "Syntax error: invalid expression in function '%s' argument at token %zu\n",
			   fname, p->count);
	}

      //Move to ',' or ')'
      tok = current(p);
      ExprNode *arg2 = NULL;

      if(tok && tok->type == TOKEN_COMMA) {
	advance_parser(p);
	arg2 = parse_expression(p);
	if(!arg2)
	  return create_error_node(NULL, NULL, "Invalid second argument in function '%s'", fname);
      }
      
      tok = current(p);

      //Advance to the next token, unless the syntax is not correct, and the function arguments are not closed by ')'
      if(!tok || tok->type != TOKEN_RPAREN) {
		return create_error_node(NULL, NULL,
				 "Syntax error: expected ')' after function argument at token number: %zu\n",
				 p->count);
      }
      
      advance_parser(p);


      return create_function_node(fname, arg1, arg2);
    }
  
  //In case of parenthesses
  if(tok->type == TOKEN_LPAREN) {
      //Move forward and build a subtree for the expression inside the parenthesses
      advance_parser(p);
      ExprNode *node = parse_expression(p);
      tok = current(p);
      if(tok && tok->type == TOKEN_RPAREN) {
	  advance_parser(p);
      } else {
	  return create_error_node(NULL, NULL,
				   "Syntax error: expected ')' after function argument at token number: %zu\n",
				   p->count);
	}
      return node;
    }
  return create_error_node(NULL, NULL,
			   "Syntax error: unexpected token in parse_primary at token number: %zu\n",
			   p->count);
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
