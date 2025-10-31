#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer_state_struct.h"
#include "token_utils.h"
#include "lexer_utils.h"
#include "builtins.h"

//Intializes a new tokenizer for a new expression
LexerState* init_lexer(const char *expr);

//Gets the current token
char get_current(LexerState *l);

//Advances the tokenizers position
void advance(LexerState *l);
void advance_offset(LexerState *l, size_t offset);

//Peeks the char with the specified offset without taking in acount the white spaces
char peek_char(LexerState *l, size_t offset);

CalcToken handle_alphabetical_identifiers(LexerState *lexer_state);

CalcToken handle_numeric_identifiers(LexerState *lexer_state);

CalcToken handle_symbols(LexerState *lexer_state);

//Converts a single char into a null terminated string(In favour of the token value member)
char* char_to_string(char c);

//Analyzes the current identifier, advances the tokens position and returns a ready token
CalcToken get_next_token(LexerState *l);

//Converts token type into a string literal
void token_to_string(const CalcToken *t, char *buf, size_t size);

//Tokenize the given math expression
CalcTokenNode* lex_expr(const char *math_expression);

//Is TokenNode of some type
int is_type(CalcTokenNode *head, CalcTokenType type);


//Prints the tokens linked list
void print_tokens(CalcTokenNode *root);



#endif
