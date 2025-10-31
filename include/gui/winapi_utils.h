#ifndef WINAPI_UTILS_H
#define WINAPI_UTILS_H

#include <stdio.h>
#include <windows.h>
#include <wchar.h>


void create_console_instance(); //Create a console instance and reroutes the stdio to the console
char* convert_wchar_to_char(const wchar_t *wbuf); //Converts wide charachter string into a C-style string(wchar_t*->char*)

#endif
