#include "lexer/lexer_utils.h"
char *empty_str()
{
  char *empty = malloc(1);
  if(empty != NULL) {
      empty[0] = '\0';
    }
  return empty;
}

size_t count_non_delim(const char *str, size_t len, char delim)
{
  if (!str) {
    return 0;
  }

  size_t count = 0;
  for (size_t i = 0; i < len; i++) {
    if (str[i] != delim) {
      count++;
    }
  }
  return count;
}

char* trim_char(const char *to_trim, size_t len, char delim)
{
  if(to_trim == NULL || len == 0) { return NULL; }

  size_t new_len = count_non_delim(to_trim, len, delim);
  if(new_len == 0) { return empty_str(); }
  
  char *trimmed_str = malloc(sizeof(char) * new_len + 1);
  if(!trimmed_str) { return NULL; }

  size_t idx_trimmed = 0, idx_non_trimmed = 0;
  while(idx_non_trimmed < len) {
      if(to_trim[idx_non_trimmed] != delim) {
	  trimmed_str[idx_trimmed++] = to_trim[idx_non_trimmed];
	}
      idx_non_trimmed++;
    }
  trimmed_str[idx_trimmed] = '\0';
  return trimmed_str;
}

char* char_to_string(char c)
{
  char *s = malloc(2);
  if (s == NULL) { return NULL; }
  s[0] = c;
  s[1] = '\0';
  return s;
}
