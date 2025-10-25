#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

typedef double (*MathFuncN)(const double *args, int argc);

typedef struct
{
    const char *name;
    MathFuncN func;
} FunctionEntry;


MathFuncN get_function(const char *name);
int f_exists(const char *name);

double f_sin(const double *args, int argc);
double f_log(const double *args, int argc);
double f_pow(const double *args, int argc);
double f_cos(const double *args, int argc);
double f_tan(const double *args, int argc);
double f_sqrt(const double *args, int argc);

#endif
