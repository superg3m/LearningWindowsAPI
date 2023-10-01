#include "../Header/Main.h"

#define CLASS_NAME "My_First_Window"

// Define constants
#define BUFFER_COUNT 2
#define BATCH_SIZE 5000

// Global variables
int index = 0;
int currentBufferIndex = 0;
int* pixelBuffers[BUFFER_COUNT];
BITMAPINFO bitmap_info;
bool windowIsRunning = false;
MSG message = {0};
HWND windowHandle = NULL;
int onlyOnce = 0;

// Function prototypes
void InitializeBuffers();
void SwapBuffers();
void Render(int* pixelData);

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quicksort algorithm
void quicksort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);  // Sort elements before the pivot
        quicksort(arr, pi + 1, high); // Sort elements after the pivot
    }
}

void quicksortNElements(int* arr, int low, int high, int nElements) {
    if (high > MAX_SIZE && onlyOnce == 0) {
        printf("WORKING\n");
        quicksort(arr, 0, MAX_SIZE);
        onlyOnce = 1;
    } else if (high < MAX_SIZE) {
        if (low < high) {
            int pi = partition(arr, low, high);

            // Check if the number of elements to sort is less than or equal to 'nElements'
            if (high - low + 1 <= nElements) {
                // If so, sort the entire subarray
                quicksortNElements(arr, low, pi - 1, nElements);
                quicksortNElements(arr, pi + 1, high, nElements);
            } else {
                // Otherwise, divide the subarray into two parts and sort each part
                if (pi - low + 1 >= nElements) {
                    quicksortNElements(arr, low, pi - 1, nElements);
                }
                if (high - pi >= nElements) {
                    quicksortNElements(arr, pi + 1, high, nElements);
                }
            }
        }
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
    registerWindowsClass(hInstance, CLASS_NAME);

    windowHandle = createWindowHandle(hInstance, CLASS_NAME);

    hdc = GetDC(windowHandle);
    RECT rect;
    GetClientRect(windowHandle, &rect);

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    MEMORY_ALLOC_SIZE = width * height * sizeof(unsigned int);

    MAX_SIZE = (MEMORY_ALLOC_SIZE / (2 * sizeof(unsigned int))) - 1;

    InitializeBuffers();

    // Initialize the first buffer
    int* start = pixelBuffers[0];
    for (int i = 0; i < MAX_SIZE; i++) {
        *(start++) = 0x0000FF + (i % 1000);
    }

    // Copy the data to the other buffers
    for (int i = 1; i < BUFFER_COUNT; i++) {
        memcpy(pixelBuffers[i], pixelBuffers[0], MEMORY_ALLOC_SIZE);
    }

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

        quicksortNElements(pixelBuffers[currentBufferIndex], 0, index, BATCH_SIZE); // Sort 100 elements at a time
        if (onlyOnce == 0) {
            index += BATCH_SIZE;
        }
        SwapBuffers();
    }

    return 0;
}

// Function to initialize pixel buffers
void InitializeBuffers() {
    for (int i = 0; i < BUFFER_COUNT; i++) {
        pixelBuffers[i] = (int*)VirtualAlloc(
            0,
            MEMORY_ALLOC_SIZE,
            MEM_RESERVE | MEM_COMMIT,
            PAGE_READWRITE
        );
    }

    // Initialize bitmap_info (assuming it's the same for both buffers)
    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info);
    bitmap_info.bmiHeader.biWidth = width;
    bitmap_info.bmiHeader.biHeight = height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
}

// Function to swap the rendering buffer
void SwapBuffers() {
    int nextIndex = (currentBufferIndex + 1) % BUFFER_COUNT;
    pixelBuffers[currentBufferIndex] = pixelBuffers[nextIndex];
    currentBufferIndex = (currentBufferIndex + 1) % BUFFER_COUNT;

    Render(pixelBuffers[currentBufferIndex]);
}

// Function to render to the current buffer
void Render(int* pixelData) {
    StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, (void*)pixelData, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);
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
