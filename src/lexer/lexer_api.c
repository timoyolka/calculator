#include "lexer/lexer_api.h"

const char *TOKEN_TYPE_NAMES[] = {
  [TOKEN_NUMBER]   = "NUMBER",
  [TOKEN_OPERATOR] = "OPERATOR",
  [TOKEN_LPAREN]   = "LPAREN",
  [TOKEN_RPAREN]   = "RPAREN",
  [TOKEN_COMMA]    = "COMMA",
  [TOKEN_FUNCTION] = "FUNCTION",
  [TOKEN_VARIABLE] = "VARIABLE",
  [TOKEN_END]      = "EXPR_END",
  [TOKEN_ERR]      = "ERR"
};

LexerState* init_lexer(const char *expr)
{
  if(!expr){
    return NULL;
  }
  
  LexerState *lexer_state = malloc(sizeof(LexerState));
  if (!lexer_state){
    return NULL;
  }
  
  lexer_state->expr = expr;
  lexer_state->pos = 0; 
  lexer_state->length = strlen(expr);
  
  return lexer_state;
}

char get_current(LexerState *lexer_state)
{
  return lexer_state->expr[lexer_state->pos];
}

void advance(LexerState *lexer_state)
{
  if(!lexer_state){ return; }
  if (lexer_state->pos < lexer_state->length) { lexer_state->pos++; }
}

void advance_offset(LexerState *lexer_state, size_t offset)
{
  for(size_t i = 0; i < offset; i++) {
      advance(lexer_state);
      if(lexer_state->pos >= lexer_state->length){
	break;
      }
    }
}

char peek_char(LexerState *lexer_state, size_t offset)
{
  size_t pos = lexer_state->pos + offset;
  if(pos >= lexer_state->length) {
    return '\0';
  }
  return lexer_state->expr[pos];
}

CalcToken handle_alphabetical_identifiers(LexerState *lexer_state)
{
  char current_char = get_current(lexer_state);
  char buffer[MAX_IDENT_LEN];
  size_t len = 0;
  while(isalpha((unsigned char)current_char) && len < MAX_IDENT_LEN - 1) {
    buffer[len++] = current_char;
    advance(lexer_state);
    current_char = get_current(lexer_state);
  }
  buffer[len] = '\0';

  if(f_exists(buffer)) {
    //advance_offset(lexer_state, len - 1);
    return create_function_token(buffer);
  }
  
  //advance(lexer_state);
  return create_variable_token(buffer);
}

CalcToken handle_numeric_identifiers(LexerState *lexer_state)
{
  char buffer[MAX_IDENT_LEN];
  int len = 0;

  char current_char = get_current(lexer_state);
  
  while ((isdigit((unsigned char)current_char) || current_char == DOT) && len < MAX_IDENT_LEN - 1) {
    buffer[len++] = current_char;
    advance(lexer_state);
    current_char = lexer_state->expr[lexer_state->pos];
  }
  buffer[len] = '\0';

  return create_numeric_token(atof(buffer));
}

CalcToken handle_symbols(LexerState *lexer_state)
{
  char current_char = get_current(lexer_state);
  char *text = char_to_string(current_char);

  if(current_char == LPAREN) { 
      advance(lexer_state);
      return create_grouping_token(TOKEN_LPAREN, text);
    }
  if(current_char == RPAREN) {
    advance(lexer_state);
    return create_grouping_token(TOKEN_RPAREN, text);
  }
  
  if(current_char == COMMA) {
    advance(lexer_state);
    return create_separator_token(text);
  }
  if(current_char == ADD || current_char == SUB ||current_char == MULT ||
     current_char == DIV || current_char == POW) {
      advance(lexer_state);
      return create_operator_token(text);
    }
}

CalcToken get_next_token(LexerState *lexer_state)
{
  if(lexer_state == NULL || lexer_state->pos >= lexer_state->length) {
    return create_end_expr_token();
  }
  
  char current_char = get_current(lexer_state);

  //Handle function names and variables
  if(isalpha(current_char)) {
    return handle_alphabetical_identifiers(lexer_state);
  }

  if (isdigit((unsigned char)current_char) || current_char == DOT) {
    return handle_numeric_identifiers(lexer_state);
  }
  
  return handle_symbols(lexer_state);
}



void token_to_string(const CalcToken *t, char *buf, size_t size)
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



CalcTokenNode *lex_expr(const char *math_expression)
{
  const char *trimmed_math_expression = trim_char(math_expression, strlen(math_expression), ' ');
  LexerState *t = init_lexer(trimmed_math_expression);
  CalcToken tok = get_next_token(t);
  
  CalcTokenNode *root = NULL;
  CalcTokenNode *curr = NULL;

  while(tok.type != TOKEN_END) {
      CalcTokenNode *new_node = malloc(sizeof(CalcTokenNode));
      new_node->t = tok;      // copy token struct
      new_node->next = NULL;
      
      if (root == NULL){
	  root = new_node;
	  curr = new_node;
	} else {
	  curr->next = new_node;
	  curr = new_node;
	}
      
      tok = get_next_token(t);
    }
  CalcTokenNode *last = malloc(sizeof(CalcTokenNode));
  last->t = tok;
  last->next = NULL;
  if(curr)
    curr->next = last;
  else
    root = last;
  
  printf("Lexed succefuly.\n");
  free(t);
  return root;
}

void print_tokens(CalcTokenNode *root)
{
  CalcTokenNode *curr = root;
  size_t buffer_size = 32;
  char *buffer = malloc(buffer_size * sizeof(char));
  while(curr != NULL) {
      token_to_string(&curr->t, buffer, buffer_size);
      printf(" %s ", buffer);
      curr = curr->next;
    }
  free(buffer);
  printf("\n");
}


