#include "builtins.h"

static const FunctionEntry FUNCTIONS[] = {
    {"sin",  f_sin},
    {"cos",  f_cos},
    {"tan",  f_tan},
    {"log",  f_log},
    {"ln",   f_ln},
    {"pow",  f_pow},
    {"sqrt", f_sqrt},
    {"abs", f_abs},
    {NULL, NULL}
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

double f_ln(const double *args, int argc)
{
    if(argc != 1) return NAN;
    if(args[0] <= 0) return NAN;
    return log(args[0]);  // natural log
}

double f_log(const double *args, int argc)
{
    if(argc == 1) {           // log base 10
        if(args[0] <= 0) return NAN;
        return log10(args[0]);
    }
    if(argc == 2) {           // log(base, number)
        if(args[0] <= 0 || args[0] == 1 || args[1] <= 0) return NAN;
        return log10(args[1]) / log10(args[0]);
    }
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

double f_abs(const double *args, int argc)
{
  if(argc != 1) return NAN;
  return fabs(args[0]);
}
