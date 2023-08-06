#include "../Header/Main.h"

/*

LRESULT CallWindowProcA(
  [in] WNDPROC lpPrevWndFunc,
  [in] HWND    hWnd,
  [in] UINT    Msg,
  [in] WPARAM  wParam,
  [in] LPARAM  lParam
);

int WinMain(
  [in] HINSTANCE hInstance,
  [in] HINSTANCE hPrevInstance,
  [in] LPSTR     lpCmdLine,
  [in] int       nShowCmd
);

*/

#define CLASS_NAME "My_First_Window"

void registerWindowsClass(HINSTANCE hInstance, const char* className);
HWND createWindowHandle(HINSTANCE hInstance, const char* className);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
  registerWindowsClass(hInstance, CLASS_NAME);

  // The first parameter of Create window is really interesting
  // WS_EX_ACCEPTFILES (The window accepts drag-drop files)
  HWND windowHandle = createWindowHandle(hInstance, CLASS_NAME);

  if (windowHandle == NULL) {
    return -1;
  }

  ShowWindow(windowHandle, nCmdShow);

  return 0;
}

void registerWindowsClass(HINSTANCE hInstance, const char* className) {
  WNDCLASSEXA windowClass = {0};
  windowClass.cbSize = sizeof(windowClass);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = DefWindowProcW;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = NULL;               // Research this more
  windowClass.hCursor = NULL;             // Research this more
  windowClass.hbrBackground = NULL;       // Research this more
  windowClass.lpszMenuName = NULL;        // Research this more
  windowClass.lpszClassName = className;  // This might be a problem
  windowClass.hIconSm = NULL;             // Research this more

  RegisterClassEx(&windowClass);
  return;
}

HWND createWindowHandle(HINSTANCE hInstance, const char* className) {
  return CreateWindowEx(0, className, "TESTING", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
}