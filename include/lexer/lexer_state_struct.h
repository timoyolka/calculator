#ifndef TOKENIZER_STRUCT_H
#define TOKENIZER_STRUCT_H

#include "lexer_conf.h"
#include <stddef.h>

typedef struct
{
  const char *expr;
  size_t pos;
  size_t length;
}LexerState;

#endif
