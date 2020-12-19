#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include <cstdio>
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI MovementThread( LPVOID lpParam );

HWND hwnd;
HICON stopped_ico, running_ico;
NOTIFYICONDATA niData; 
bool running;

DWORD WINAPI MovementThread( LPVOID lpParam ) 
{
    int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    while (true) {
        while (running) {
            int x = rand()*w/RAND_MAX;
            int y = rand()*h/RAND_MAX;
            printf("%i %i\n",x,y);
            SetCursorPos(x,y);
            keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
            Sleep(1000*10);
        }
        Sleep(1000);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int _nCmdShow)
{
    const wchar_t CLASS_NAME[]  = L"MouseMove Window Class";   
    WNDCLASS wc = { };
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    //dummy window (never shown)
    hwnd = CreateWindowEx(NULL, CLASS_NAME, L"MouseMove", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 250, 80, NULL, NULL, hInstance, NULL);
    
    running = false;
    CreateThread(NULL, 0, MovementThread, NULL, 0, NULL);

    running_ico = (HICON)LoadIcon(hInstance,MAKEINTRESOURCE(1));
    stopped_ico = (HICON)LoadIcon(hInstance,MAKEINTRESOURCE(2));
    
    ZeroMemory(&niData,sizeof(NOTIFYICONDATA));
    niData.cbSize = sizeof(NOTIFYICONDATA);
    niData.uID = 42;
    niData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
    niData.hIcon = stopped_ico;
    niData.hWnd = hwnd;
    niData.uCallbackMessage = WM_APP;
    Shell_NotifyIcon(NIM_ADD,&niData);
    
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

void toggle() {
    running = !running;
    printf("toggle\n");
    if (running) {
        niData.hIcon = running_ico;
    } else { 
        niData.hIcon = stopped_ico;
    }
    Shell_NotifyIcon(NIM_MODIFY,&niData);
}

void quit() {
    Shell_NotifyIcon(NIM_DELETE,&niData);
    PostQuitMessage(0);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_APP:
            switch(lParam)
            {
                case WM_RBUTTONDOWN:
                    quit();
                    break;
                case WM_LBUTTONDOWN:
                    toggle();
                    break;
            }
            return 0;
            
        case WM_COMMAND:
            toggle();
            return 0;
            
        case WM_DESTROY:
            quit();
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                EndPaint(hwnd, &ps);
            }
            return 0;
            
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
