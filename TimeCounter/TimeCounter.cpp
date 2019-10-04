// TimeCounter.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TimeCounter.h"
#include "WindowTrackingService.h"
#include "StackPanel.h"
#include "Psapi.h"
#include "FileSerializer.h"
#include <filesystem>

#define MAX_LOADSTRING 100
#define TIMER_WINDOWCHANGE 500
#define TIMER_SAVETOFILE 2000
#define WINDOWCAPTION_MAXL 200

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

WindowTrackingService* windowTrackingService;
StackPanel* stackPanel;
FileSerializer* serializer;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIMECOUNTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMECOUNTER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMECOUNTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIMECOUNTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void CALLBACK OnSaveToFile(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime);

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetTimer(hWnd, NULL, TIMER_WINDOWCHANGE, NULL);
   SetTimer(NULL, NULL, TIMER_SAVETOFILE, (TIMERPROC) &OnSaveToFile);

   windowTrackingService = new WindowTrackingService();
   stackPanel = new StackPanel(windowTrackingService, hWnd);
   serializer = new FileSerializer();

   auto dataFromDisk = serializer->Deserialize();

   windowTrackingService->OnDeserialization(dataFromDisk);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void CALLBACK OnSaveToFile(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	serializer->Serialize(*windowTrackingService->TrackedTime);
}


std::wstring FileName(const std::wstring& str)
{
	size_t found = str.find_last_of(L"/\\");
	std::wstring path = str.substr(found + 1);
	return path;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

HWND CurrentWindowHwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			auto panels = *stackPanel->GetDrawablePanels();

			for (auto panel : panels) {
				if (panel.hasIcon) {
					DrawIconEx(hdc, panel.icon.xPos, panel.icon.yPos, panel.icon.icon, panel.icon.width, panel.icon.height, 0, 
						NULL, DI_NORMAL | DI_COMPAT | DI_DEFAULTSIZE);
				}
				else {
					TextOut(hdc, panel.icon.xPos, panel.icon.yPos, L"App is dead.", sizeof(L"App is dead.") / 2);
				}

				TextOut(hdc, panel.textxPos, panel.textyPos, (LPCWSTR)panel.appName->c_str(), panel.appName->length());
				TextOut(hdc, panel.text2xPos, panel.text2yPos, (LPCWSTR)panel.timeSpent->c_str(), panel.timeSpent->length());
				delete panel.timeSpent;
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_TIMER: {
		auto currentWindow = GetForegroundWindow();

		if (currentWindow != CurrentWindowHwnd) {
			DWORD pid;

			GetWindowThreadProcessId(currentWindow, &pid);

			HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE,
				pid);

			wchar_t filenameBuffer[1024];
			GetProcessImageFileName(h, filenameBuffer, 1024);

			auto bufferString = new std::wstring(filenameBuffer);
			auto fileName = FileName(*bufferString);
			bufferString->clear();
			bufferString->append(fileName);
			windowTrackingService->OnWindowChanged(bufferString, currentWindow);
			CurrentWindowHwnd = currentWindow;	
		}

		windowTrackingService->OnTimerTick(TIMER_WINDOWCHANGE);
		InvalidateRect(hWnd, NULL, true);
	}
		break;
	case WM_QUIT:
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
