#include "builtins.h"

static const FunctionEntry FUNCTIONS[] = {
    {"sin",  f_sin},
    {"cos",  f_cos},
    {"tan",  f_tan},
    {"log",  f_log},
    {"ln",   f_log},   /* alias for natural log */
    {"pow",  f_pow},
    {"sqrt", f_sqrt},
    {NULL, NULL}       /* sentinel */
};


MathFuncN get_function(const char *name)
{
    if (!name)
        return NULL;

    for (int i = 0; FUNCTIONS[i].name != NULL; i++)
    {
        if (strcmp(FUNCTIONS[i].name, name) == 0)
            return FUNCTIONS[i].func;
    }
    return NULL;
}

int f_exists(const char *name)
{
  if(!name)
    return 0;

  for(int i = 0; FUNCTIONS[i].name != NULL; i++)
    {
      if(strcmp(FUNCTIONS[i].name, name) == 0)
	return 1;
    }
  return 0;
}

double f_sin(const double *args, int argc)
{
  if (argc != 1) return NAN;
  return sin(args[0]);
}

double f_log(const double *args, int argc)
{
  if (argc == 1) return log(args[0]);        // natural log
  if (argc == 2) return log(args[0]) / log(args[1]); // log base n
  return NAN;
}

double f_pow(const double *args, int argc)
{
  if (argc != 2) return NAN;
  return pow(args[0], args[1]);
}

double f_cos(const double *args, int argc)
{
  if (argc != 1) return NAN;
  return cos(args[0]);
}

double f_tan(const double *args, int argc)
{
  if(argc != 1) return NAN;
  return tan(args[0]);
}


double f_sqrt(const double *args, int argc)
{
  if (argc != 1) return NAN;
  return sqrt(args[0]);
}
