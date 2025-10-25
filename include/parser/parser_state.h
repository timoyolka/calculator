#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "lexer/lexer_api.h"
#include "parser/expr_node.h"
#include "builtins.h"

typedef struct {
  TokenNode *current;
  size_t count;
}ParserState;

//Initializes the parser
void init_parser(ParserState *p, TokenNode *head);

//Gets the current term's token
Token* current(const ParserState *p);

//Consumes the current token
void advance_parser(ParserState *p);

#endif
