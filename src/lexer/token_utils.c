#include "lexer/token_utils.h"

CalcToken create_end_expr_token()
{
  CalcToken tok = {
    .type = TOKEN_END
  };
  return tok;
}

CalcToken create_function_token(const char* f_name)
{
  if(f_name == NULL) {
    return create_end_expr_token();
  }
  
  char *text = strdup(f_name);
  if(text == NULL) {
    return create_end_expr_token();
  }
  
  CalcToken tok = {
    .type = TOKEN_FUNCTION,
    .value.text = text
  };
  return tok;
}

CalcToken create_variable_token(const char* v_name)
{
  if(v_name == NULL) {
    return create_end_expr_token();
  }

  char *text = strdup(v_name);
  if(text == NULL) {
    return create_end_expr_token();
  }
  
  CalcToken tok = {
    .type = TOKEN_VARIABLE,
    .value.text = text
  };
  return tok;
}

CalcToken create_numeric_token(double val)
{
  CalcToken tok = {
    .type = TOKEN_NUMBER,
    .value.number = val
  };
  return tok;
}

CalcToken create_grouping_token(CalcTokenType paren_type, const char *paren)
{
  if(paren == NULL) {
    return create_end_expr_token();
  }

  char *text = strdup(paren);
  if(text == NULL) {
    return create_end_expr_token();
  }
  
  CalcToken tok = {
    .type = paren_type,
    .value.text = text
  };
  return tok;
}

CalcToken create_separator_token(const char *comma)
{
  if(comma == NULL) {
    return create_end_expr_token();
  }

  char *text = strdup(comma);
  if(text == NULL) {
    return create_end_expr_token();
  }
  
  CalcToken tok = {
    .type = COMMA,
    .value.text = text
  };
  return tok;
}

CalcToken create_operator_token(const char *op)
{
  if(op == NULL)
    {
      return create_end_expr_token();
    }

  char *text = strdup(op);
  if(text == NULL) {
    return create_end_expr_token();
  }
  
  CalcToken tok = {
    .type = TOKEN_OPERATOR,
    .value.text = text
  };
  return tok;
}
