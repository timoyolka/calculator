#ifndef TOKEN_STRUCT_H
#define TOKEN_STRUCT_H

#include "token_types.h"
#include <stddef.h> //for NULL

typedef struct
{
  CalcTokenType type;
  union
  {
    char *text;
    double number;
  }value;
}CalcToken;

typedef struct CalcTokenNode
{
  CalcToken t;
  struct CalcTokenNode *next;
}CalcTokenNode;

#endif
