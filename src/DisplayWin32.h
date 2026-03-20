#pragma once

#include <windows.h>
#include <WinUser.h>
#include "InputDevice.h"

class Game;

class DisplayWin32
{
private: 
    HINSTANCE hInstance;
    WNDCLASSEX wc;
public:
    static DisplayWin32* Instance;
    int ClientHeight;
    int ClientWidth;
    
    HWND hWnd;
    
    DisplayWin32(LPCWSTR applicationName, Game* game, int width = 800, int height = 800);
    DisplayWin32();
    ~DisplayWin32();
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};