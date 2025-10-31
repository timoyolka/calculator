#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <string.h>

#include "lexer/token_struct.h"

CalcToken create_end_expr_token();
CalcToken create_function_token(const char* f_name);
CalcToken create_variable_token(const char* v_name);
CalcToken create_numeric_token(double val);
CalcToken create_grouping_token(CalcTokenType paren_type, const char *paren);
CalcToken create_separator_token(const char *comma);
CalcToken create_operator_token(const char *op);

#endif 
