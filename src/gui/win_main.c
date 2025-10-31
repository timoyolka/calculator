#include "gui/win_main.h"

WindowData display_wnd = {
  .hwnd = NULL,
  .hWndOldProc = NULL
};

WindowData CreateChildWnd(WNDPROC newProc,
			  LPCWSTR lpClassName,  LPCWSTR lpWindowName, LPCWSTR wndTxt,
			  DWORD dwExStyle, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
  /*
    That's another Win32 idiom, lParam of WM_CREATE is a pointer to a CREATESTRUCT instance.
    CREATESTRUCT contains all the info that was passed to CreateWindowExW when the parent window was created.
    hInstance is the HISNTACE(instance-handle) o the application that created the parrent window.
    CreateWindowExW(...) requires the HISTANCE of the module(every .exe or .dll gets an HINSTANCE/HMODULE) that owns the window class.
  */
  HWND hWndChild = CreateWindowExW(dwExStyle, lpClassName, lpWindowName,
			      WS_CHILD | dwStyle,
			      X, Y, nWidth, nHeight,
			      hWndParent, hMenu, hInstance,
			      lpParam
			      );
      
  SetWindowTextW(hWndChild, wndTxt);
  WNDPROC hWndChildOldProc = NULL;
  if(newProc)
    {
      hWndChildOldProc = (WNDPROC)SetWindowLongPtrW(
						    hWndChild,
						    GWLP_WNDPROC,
						    (LONG_PTR)newProc
						    );
    }
  ShowWindow(hWndChild, SW_SHOW);
  if(!UpdateWindow(hWndChild))
    {
      MessageBoxW(NULL, L"UpdateWindow failed!", L"Error", MB_OK);
    }
  return (WindowData){hWndChild, hWndChildOldProc};
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  if(wcscmp(WC_FLAG_DEBUG_CONSOLE, lpCmdLine) == 0)
    {
      create_console_instance();
    }
  // Register the window class.
  LPCWSTR CLASS_NAME = WINDOW_CLASS_NAME;
    
  WNDCLASSEXW wc = { };
  wc.cbSize        = sizeof(WNDCLASSEXW);
  wc.lpfnWndProc   = WindowProc;
  wc.hInstance     = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  /*
    a winapi trick, win32 expects the value 1..N to represent system color brushes,
    the +1 is required because internally '0' means "no brush" and Windows offset system colors by 1.
    The cast just tells the compiler, "trust me, this integer is a valid HBRUSH handle in this special case
    Could have used CreateSolidBrush(RGB(255,255,255)) instead
  */

  if(!RegisterClassExW(&wc)) {
    MessageBoxW(NULL, L"RegisterClassExW failed!", L"Error", MB_OK);
    return 0;
  }

  // Create the window.

  HWND hwnd = CreateWindowExW(0, CLASS_NAME, WINDOW_NAME, // Window text
			      WS_OVERLAPPEDWINDOW, //Styles
			      CW_USEDEFAULT, CW_USEDEFAULT, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, //size and pos
			      NULL,       // Parent wnd    
			      NULL,       // Menu
			      hInstance,  // Instance handle
			      NULL        // Additional data
			      );

  if (hwnd == NULL) {
    MessageBoxW(NULL, L"CreateWindowExW failed!", L"Error", MB_OK);
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // Run the message loop.
  MSG msg = { };
  while (GetMessage(&msg, NULL, 0, 0) > 0){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
    {
    case WM_CREATE:
      /*
	That's another Win32 idiom, lParam of WM_CREATE os a pointer to a CREATESTRUCT instance.
	CREATESTRUCT contains all the info that was passed to CreateWindowExW when the parent window was created.
	hInstance is the HISNTACE(instance-handle) o the application that created the parrent window.
	CreateWindowExW(...) requires the HISTANCE of the module(every .exe or .dll gets an HINSTANCE/HMODULE) that owns the window class.
      */
      display_wnd = CreateChildWnd(EditDisplaySubclassWindowProc,
				   L"EDIT", L"", L"", 0,
				   WS_VISIBLE | WS_BORDER | ES_LEFT,
				   10, 10, 260, 50,
				   hwnd,
				   (HMENU)IDC_DISPLAY,
				   ((LPCREATESTRUCT)lParam)->hInstance,
				   NULL
				   );
      break;
      
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
      break;
      
    case WM_PAINT:
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      // All painting occurs here, between BeginPaint and EndPaint.

      FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

      EndPaint(hwnd, &ps);
      return 0;
      break;
      
    case WM_SIZE:
      if(display_wnd.hwnd) {
	MoveWindow(display_wnd.hwnd, 10, 10, LOWORD(lParam) - 20, 50, 1);
      }
      break;
    }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditDisplaySubclassWindowProc(HWND sub_class_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
    {
    case WM_KEYDOWN:
      if(wParam == VK_RETURN) {
	wchar_t wbuf[DISPLAY_BUFF_SIZE];
	GetWindowTextW(sub_class_hwnd, (LPWSTR)wbuf, DISPLAY_BUFF_SIZE);
	char *buf = convert_wchar_to_char(wbuf);

	ParserState p;
	CalcTokenNode *head = lex_expr(buf);
	if(!head) {
	  printf("Lexing failed.\n");
	  return 0;
	}
	print_tokens(head);
	  
	init_parser(&p, head);
	ExprNode *root = parse_expression(&p);
	if(!root) {
	  printf("Parsing failed.\n");
	}
	  
	print_tree_pyramid(root);
	  
	double res = eval_tree(root);

	swprintf(wbuf, DISPLAY_BUFF_SIZE, L"%g", res);
	SetWindowTextW(sub_class_hwnd, (LPWSTR)wbuf);
	SetFocus(GetParent(sub_class_hwnd));
      }
      return 0;
      break;
    }
  return CallWindowProcW(display_wnd.hWndOldProc, sub_class_hwnd, uMsg,  wParam, lParam);
}



