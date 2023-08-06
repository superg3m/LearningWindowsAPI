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

// Function Prototypes
void registerWindowsClass(HINSTANCE hInstance, const char* className);
HWND createWindowHandle(HINSTANCE hInstance, const char* className);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
  bool windowIsRunning = false;
  // char response[4];
  MSG message = {0};

  registerWindowsClass(hInstance, CLASS_NAME);

  // The first parameter of Create window is really interesting
  // WS_EX_ACCEPTFILES (The window accepts drag-drop files)
  HWND windowHandle = createWindowHandle(hInstance, CLASS_NAME);

  if (windowHandle == NULL) {
    return -1;
  }
  windowIsRunning = true;

  ShowWindow(windowHandle, nCmdShow);

  while (windowIsRunning) {
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        windowIsRunning = false;
        break;
      }
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Add a short delay to give the window time to respond to user interactions
    Sleep(10);  // Adjust the duration as needed
  }

  return 0;
}

void registerWindowsClass(HINSTANCE hInstance, const char* className) {
  WNDCLASSEXA windowClass = {0};
  windowClass.cbSize = sizeof(windowClass);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProc;
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_SIZE: {
      int width = LOWORD(lParam);
      int height = HIWORD(lParam);
      OnSize(hwnd, (UINT)wParam, width, height);
    } break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);

      FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

      EndPaint(hwnd, &ps);
    } break;

    case WM_CLOSE: {
      DestroyWindow(hwnd);
    } break;

    case WM_DESTROY: {
      PostQuitMessage(0);  // Exits the message loop
    } break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void OnSize(HWND hwnd, UINT flag, int width, int height) {
  // Handle resizing
}