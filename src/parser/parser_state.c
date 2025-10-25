#include "parser/parser_state.h"

void init_parser(ParserState *p, TokenNode *head)
{
  if(!p) { return; }
  p->current = head;
  p->count = 0;
}

//Parser state control functions
Token* current(const ParserState *p)
{
  if(p->current)
    return &p->current->t;
  return NULL;
}

void advance_parser(ParserState *p)
{
  if(p->current != NULL && p->current->next != NULL)
    {
      p->current = p->current->next;
      p->count++;
    }
}
