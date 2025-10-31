#include "gui/winapi_utils.h"

void create_console_instance()
{
  //Create a new console window
  AllocConsole();

  //Reroute the stdio to the new console instance
  FILE *fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
  freopen_s(&fp, "CONOUT$", "r", stdin);

  printf("Debug console initialized.\n");
}

char* convert_wchar_to_char(const wchar_t *wbuf)
{
  //get required buffer size (in bytes, including terminating NUL)
  int needed = WideCharToMultiByte(
				   CP_UTF8, //Target encoding
				   0,       //Conversion options
				   wbuf,    //src wchar_t* string
				   -1,      //src string len
				   NULL,    //dest buf(NULL to get size)
				   0,       //dest buf size
				   NULL,    //replcement char if fails
				   NULL     //output flag
				   );
  if (needed == 0) {
    DWORD err = GetLastError();
    fprintf(stderr, "wchar_t* -> char* failed (size query): %lu\n", err);
    return "";
  }
  
  char *buf = (char*)malloc(needed);
  if (!buf) {
    fprintf(stderr, "malloc failed\n"); return "";
  }

  int written = WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, needed, NULL, NULL);
  if (written == 0) {
    DWORD err = GetLastError();
    fprintf(stderr, "wchar_t* -> char* failed (convert): %lu\n", err);
    return "";
  } else {
    printf("Converted (%d bytes): %s\n", written, buf);    
  }
  return buf;
}
