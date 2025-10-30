#include "lexer/lexer_api.h"

const char *TOKEN_TYPE_NAMES[] = {
    [TOKEN_NUMBER] = "NUMBER",
    [TOKEN_OPERATOR] = "OPERATOR",
    [TOKEN_LPAREN] = "LPAREN",
    [TOKEN_RPAREN] = "RPAREN",
    [TOKEN_COMMA] = "COMMA",
    [TOKEN_FUNCTION] = "FUNCTION",
    [TOKEN_VARIABLE] = "VARIABLE",
    [TOKEN_END] = "EXPR_END",
    [TOKEN_ERR] = "ERR"};

LexerState *init_lexer(const char *expr)
{
  if (!expr)
    return NULL;

  LexerState *l = malloc(sizeof(LexerState)); // I hate ifs with no {}
  if (!l)                                     
    return NULL;

  l->expr = expr;
  l->pos = 0;
  l->length = strlen(expr);

  return l;
}

char get_current(LexerState *l)
{
  return l->expr[l->pos];
}

void advance(LexerState *l)
{
  if (!l)
  {
    return;
  }
  if (l->pos < l->length)
  {
    l->pos++;
  }
}

void advance_offset(LexerState *l, size_t offset)
{
  for (size_t i = 0; i < offset; i++)
  {
    advance(l);
    if (l->pos >= l->length)
      break;
  }
}

char peek_nonspace_char(LexerState *l, size_t offset)
{
  size_t pos = l->pos + offset;
  while (pos < l->length && isspace((unsigned char)l->expr[pos]))
    pos++;

  return pos < l->length ? l->expr[pos] : '\0';
}

void skip_space(LexerState *l)
{
  if (!l)
    return;
  while (isspace(l->expr[l->pos]) && l->pos < l->length)
    l->pos++;
}

char *char_to_string(char c)
{
  char *s = malloc(2);
  if (!s)
    return NULL;
  s[0] = c;
  s[1] = '\0';
  return s;
}

// l is a bad name. The rule of thumb is: if you leave your code for 1 year would you know what 'l' is?

// This function is hard. Hard to read, follow and understand.
// You could easily splited this into atleast 4 functions
// It's hard to follow when `advance` happends and why and the real problem is that you have too many returns.
// with better function design you will be able to make this whole function look better
Token get_next_token(LexerState *l)
{
  Token token = {0}; // initialize
  // You do `!l` a lot and it's not a mistake but it's wrong logically.
  // l is a pointer and ! refers to a bool. The better way of doing it would be l != NULL.
  // You can also make a macro for it
  if (!l || l->pos >= l->length)
  {
    token.type = TOKEN_END;
    return token;
  }

  skip_space(l); // Why don't you just remove it before parsing

  char current_char = get_current(l); // This function is meaningless

  // Handle function names and variables
  if (isalpha(current_char))
  {
    char buffer[MAX_IDENT_LEN];
    size_t len = 0;
    while (isalpha((unsigned char)current_char) && len < MAX_IDENT_LEN - 1)
    {
      buffer[len++] = current_char;
      current_char = peek_nonspace_char(l, len);
    }
    buffer[len] = '\0';

    if (f_exists(buffer))
    {
      advance_offset(l, len);
      token.type = TOKEN_FUNCTION;
      token.value.text = strdup(buffer);
      return token;
    }

    current_char = get_current(l);
    token.type = TOKEN_VARIABLE;
    token.value.text = char_to_string(current_char);
    advance(l);
    return token;
  }

  if (isdigit((unsigned char)current_char) || current_char == '.')
  {
    char buffer[MAX_IDENT_LEN];
    int len = 0;

    while ((isdigit((unsigned char)current_char) || current_char == '.') && len < MAX_IDENT_LEN - 1)
    {
      buffer[len++] = current_char;
      advance(l);
      current_char = l->expr[l->pos];
    }
    buffer[len] = '\0';

    token.type = TOKEN_NUMBER;
    token.value.number = atof(buffer);
    return token;
  }

  char *text = char_to_string(current_char);

  // Handle parentheses
  if (current_char == LPAREN)
  {
    token.type = TOKEN_LPAREN;
    token.value.text = text;

    advance(l);
    return token;
  }
  if (current_char == RPAREN)
  {
    token.type = TOKEN_RPAREN;
    token.value.text = text;

    advance(l);
    return token;
  }
  if (current_char == COMMA)
  {
    token.type = TOKEN_COMMA;
    token.value.text = text;
  }

  if (current_char == ADD || current_char == SUB || current_char == MULT ||
      current_char == DIV || current_char == POW)
  {
    token.type = TOKEN_OPERATOR;
    token.value.text = text;
    advance(l);
    return token;
  }

  advance(l);
  return token;
}

void token_to_string(const Token *t, char *buf, size_t size)
{
  const char *token_type = TOKEN_TYPE_NAMES[t->type];
  const char *text_val = NULL;
  double number_val = 0.0;

  if (t->type == TOKEN_NUMBER)
    number_val = t->value.number;
  else
    text_val = t->value.text;

  if (text_val)
    snprintf(buf, size, "[%s : %s]", token_type, text_val);
  else
    snprintf(buf, size, "[%s : %g]", token_type, number_val);
}

TokenNode *lex_expr(const char *math_expression)
{
  LexerState *t = init_lexer(math_expression);
  Token tok = get_next_token(t);

  TokenNode *root = NULL;
  TokenNode *curr = NULL;

  while (tok.type != TOKEN_END)
  {
    TokenNode *new_node = malloc(sizeof(TokenNode));
    new_node->t = tok; // copy token struct
    new_node->next = NULL;

    if (root == NULL)
    {
      root = new_node;
      curr = new_node;
    }
    else
    {
      curr->next = new_node;
      curr = new_node;
    }

    tok = get_next_token(t);
  }
  TokenNode *last = malloc(sizeof(TokenNode));
  last->t = tok;
  last->next = NULL;
  if (curr)
    curr->next = last;
  else
    root = last;

  printf("Lexed succefuly.\n");
  free(t);
  return root;
}

size_t count_tokens(TokenNode *head, int (*predicate)(TokenNode *, TokenType), TokenType type)
{
  if (!head)
  {
    return 0;
  }

  size_t count = 0;
  TokenNode *curr = head;
  while (curr != NULL)
  {
    if (predicate == NULL ? 1 : predicate(curr, type))
      count++;
    curr = curr->next;
  }
  return count;
}

int is_type(TokenNode *head, TokenType type)
{
  return head->t.type == type;
}

void print_tokens(TokenNode *root)
{
  TokenNode *curr = root;
  size_t buffer_size = 32;
  char *buffer = malloc(buffer_size * sizeof(char));
  while (curr != NULL)
  {
    token_to_string(&curr->t, buffer, buffer_size);
    printf(" %s ", buffer);
    curr = curr->next;
  }
  free(buffer);
  printf("\n");
}
