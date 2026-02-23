#pragma once

#include <windows.h>
#include <WinUser.h>

class DisplayWin32
{
private:
    LPCWSTR applicationName = L"Waht";
public:
    int ClientHeight = 360;
    int ClientWidth = 640;
    HINSTANCE hInstance;
    HWND hWnd;
    // Module?
    WNDCLASSEX wc;
    
    DisplayWin32(LRESULT (CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM));
};