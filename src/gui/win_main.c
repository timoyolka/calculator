#include "gui/win_main.h"

static HWND h_edit_calc_input;
static WNDPROC old_edit_calc_proc = NULL;


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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
  if(wcscmp(L"-debug_console", lpCmdLine) == 0)
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
      h_edit_calc_input = CreateWindowExW(0, L"EDIT", L"",
					  WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
					  10, 10, 260, 50,
					  hwnd,
					  (HMENU)IDC_DISPLAY,
					  ((LPCREATESTRUCT)lParam)->hInstance,
					  NULL
					  );
      
      SetWindowTextW(h_edit_calc_input, L"");
      old_edit_calc_proc = (WNDPROC)SetWindowLongPtrW(
						      h_edit_calc_input,
						      GWLP_WNDPROC,
						      (LONG_PTR)EditSubclassWindowProc
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
      if(h_edit_calc_input) {
	MoveWindow(h_edit_calc_input, 10, 10, LOWORD(lParam) - 20, 50, 1);
      }
      break;
    }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditSubclassWindowProc(HWND sub_class_hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
  return CallWindowProcW(old_edit_calc_proc, sub_class_hwnd, uMsg,  wParam, lParam);
}
