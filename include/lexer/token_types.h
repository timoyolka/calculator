#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

       #define LPAREN '('
       #define RPAREN ')'
       #define COMMA ','

	#define ADD '+'
	#define SUB '-'
	#define MULT '*'
	#define DIV '/'
	#define POW '^'

	#define MAX_TYPE_NAME_SIZE 8

	typedef enum
	  {
		TOKEN_NUMBER,      // numeric literal (e.g. 3.14)
		TOKEN_OPERATOR,    // +, -, *, /, ^
		TOKEN_LPAREN,      // (
		TOKEN_RPAREN,      // )
		TOKEN_COMMA,
		TOKEN_FUNCTION,    // sin, cos, sqrt, etc.
		TOKEN_VARIABLE,    // optional, e.g. x
		TOKEN_END,         // end of expression / string terminator
		TOKEN_ERR
	  }TokenType;

	extern const char *TOKEN_TYPE_NAMES[];

#endif
