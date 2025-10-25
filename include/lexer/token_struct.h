#ifndef TOKEN_STRUCT_H
#define TOKEN_STRUCT_H

#include "token_types.h"
#include <stddef.h> //for NULL

typedef struct
{
  TokenType type;
  union
  {
    char *text;
    double number;
  }value;
}Token;

typedef struct TokenNode
{
  Token t;
  struct TokenNode *next;
}TokenNode;

#endif
