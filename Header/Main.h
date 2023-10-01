#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

// Function Prototypes
void registerWindowsClass(HINSTANCE hInstance, const char* className);
HWND createWindowHandle(HINSTANCE hInstance, const char* className);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnSize(HWND hwnd, UINT flag, int width, int height);

int width = 0;
int height = 0;
void* memory;
HDC hdc;

int MEMORY_ALLOC_SIZE = 0;
int HALF_SIZE = 0;