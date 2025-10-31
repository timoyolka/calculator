#ifndef WIN_MAIN_H
#define WIN_MAIN_H

#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

#include "gui/winapi_utils.h"
#include "lexer/lexer_api.h"
#include "parser/ast_parse.h"
#include "parser/ast_eval.h"
#include "parser/ast_printer.h"

//basic C type in winapi
/*
  BYTE	        unsigned char	         8-bit
  WORD	        unsigned short	 16-bit
  DWORD	unsigned long	        32-bit
  LONG	        long signed           32-bit (on 32-bit Windows)
  BOOL	        int  TRUE (1) / FALSE (0)
  CHAR	        char	               ANSI character
  WCHAR	wchar_t	        Wide character (usually UTF-16 in Windows)
  TCHAR	char or wchar_t	Depends on UNICODE macro (ANSI or Unicode)
  LPSTR	CHAR*	              Pointer to ANSI string
  LPCSTR	const CHAR*	       Pointer to constant ANSI string
  LPWSTR	WCHAR*	Pointer      to wide string
  LPCWSTR	const WCHAR*	      Pointer to constant wide string
  LPTSTR	TCHAR*	Pointer      to TCHAR string (ANSI or Unicode)
  LPCTSTR	const TCHAR*	      Pointer to constant TCHAR string
*/

//Pointers and handles
/*
  Type	        Meaning	Notes
  HANDLE	void*	       Generic handle to object (files, windows, threads, etc.)
  HWND	HANDLE	Window         handle
  HINSTANCE	HANDLE	       Instance handle of the application
  HDC	HANDLE	Device         context handle
  HMENU	HANDLE	       Menu handle
  HICON	HANDLE	       Icon handle
  HBRUSH	HANDLE	       Brush handle
 */

//Special pointer types
/*
  Type	       Meaning
  LPVOID	void* (generic pointer)
  LPCVOID	const void*
  PVOID	void* (sometimes same as LPVOID)
  LPARAM	LONG_PTR (message parameter, can store pointer or integer)
  WPARAM	UINT_PTR (message parameter, usually unsigned)
  LRESULT	LONG_PTR (return value of WindowProc)
 */

//Windows string/character types
/*
  Type	       Meaning
  CHAR	        ANSI 8-bit character
  WCHAR	UTF-16 16-bit character (wide char)
  TCHAR	Conditional: ANSI or wide char
  LPSTR	Pointer to ANSI string
  LPWSTR	Pointer to wide string
  LPTSTR	Pointer to TCHAR string
  LPCSTR	Constant ANSI string
  LPCWSTR	Constant wide string
  LPCTSTR	Constant TCHAR string
 */

//Misc common types
/*
  UINT	        unsigned int
  INT	        int
  SHORT	short
  ULONG	unsigned long
  LONG_PTR	long (or 64-bit on x64) for pointer-sized integers
  UINT_PTR	unsigned long (or 64-bit) for pointer-sized unsigned integers
*/


#define WINDOW_CLASS_NAME L"Calculator"
#define WINDOW_NAME L"Calculator"

#define MAIN_WINDOW_HEIGHT 700
#define MAIN_WINDOW_WIDTH 500

#define DISPLAY_BUFF_SIZE 256

#define IDC_DISPLAY 2

typedef struct buttonTag {
  int id;
  wchar_t *name;
}buttonTag;

typedef enum{
  IDC_BUTTON_EQULA = 1000, 
  IDC_BUTTON_SHIFT, 
  IDC_BUTTON_OPTION, 
  IDC_BUTTON_MENU,
  IDC_BUTTON_EXIT, 
  IDC_BUTTON_POWER, 
  IDC_BUTTON_VAR, 
  IDC_BUTTON_LOG, 
  IDC_BUTTON_LN, 
  IDC_BUTTON_SIN, 
  IDC_BUTTON_COS, 
  IDC_BUTTON_TAN, 
  IDC_BUTTON_ROOT, 
  IDC_BUTTON_LPAREN, 
  IDC_BUTTON_RPAREN, 
  IDC_BUTTON_ANS
}panelButtonId;

typedef enum{
  IDC_BUTTON_MULT = IDC_BUTTON_ANS + 1,
  IDC_BUTTON_DIV,
  IDC_BUTTON_PLUS,
  IDC_BUTTON_MINUS,
  IDC_BUTTON_DEL,
}opButtonId;

typedef enum{
  IDC_BUTTON_ZERO = IDC_BUTTON_DEL + 1,
  IDC_BUTTON_ONE,
  IDC_BUTTON_TWO,
  IDC_BUTTON_THREE,
  IDC_BUTTON_FOUR,
  IDC_BUTTON_FIVE,
  IDC_BUTTON_SIX,
  IDC_BUTTON_SEVEN,
  IDC_BUTTON_EIGHT,
  IDC_BUTTON_NINE
}numPadButtonId;

#define BUTTON_N (IDC_BUTTON_NINE - IDC_BUTTON_EQULA)

typedef struct{
  HWND *panel;
  HWND *op;
  HWND *num_pad;
}buttonContainer;

typedef struct{
  HWND hwnd;
  WNDPROC hWndOldProc;
}WindowData;
  
#define IDC_BUTTON_PI 400
#define IDC_BUTTON_EULER 401
#define IDC_BUTTON_GOLDEN_RATION 402




#define WC_FLAG_DEBUG_CONSOLE L"-debug_console"

WindowData CreateChildWnd(WNDPROC newProc, LPCWSTR lpClassName, LPCWSTR lpWindowName, LPCWSTR wndTxt, DWORD dwExStyle, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

//Main windows -> Display, Numpad, FuncPad, OpPad
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //Main window process
LRESULT CALLBACK EditDisplaySubclassWindowProc(HWND sub_class_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //Child window display process
//LRESULT CALLBACK NumPadSubclassWindowProc(HWND sub_class_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //Child NumPad process

  
#endif
