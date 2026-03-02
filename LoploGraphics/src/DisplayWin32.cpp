#include "DisplayWin32.h"
#include "Game.h"

LRESULT CALLBACK DisplayWin32::WndProc(HWND hWnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
    Game* game = (Game*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    if (!game) {
        return DefWindowProc(hWnd, umessage, wparam, lparam);
    }
    
	switch (umessage)
	{
	   case WM_DESTROY:
	       PostQuitMessage(0);
	       return 0;
	   case WM_INPUT:
	   {
	       UINT inputBufferSize = 0;
	       GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &inputBufferSize, sizeof(RAWINPUTHEADER));
	       if (!inputBufferSize)
	       {
	           return DefWindowProc(hWnd, umessage, wparam, lparam);
	       }
	       std::vector<BYTE> inputBuffer(inputBufferSize);
	       
	       if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, inputBuffer.data(), &inputBufferSize, sizeof(RAWINPUTHEADER)) == inputBufferSize)
	       {
	           RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(inputBuffer.data());
	           
	           if(raw->header.dwType == RIM_TYPEKEYBOARD)
	           {
	               InputDevice::KeyboardInputEventArgs args;
	               args.MakeCode = raw->data.keyboard.MakeCode;
	               args.Flags = raw->data.keyboard.Flags;
	               args.VKey = raw->data.keyboard.VKey;
	               args.Message = raw->data.keyboard.Message;
                   game->Input.OnKeyDown(args);
	           }
               // else if(raw->header.dwType == RIM_TYPEMOUSE)
               // {
	           
               // }
               PRAWINPUT pRawInput = raw;
               DefRawInputProc(&pRawInput, 1, sizeof(RAWINPUTHEADER));
	       }
	       return 0;
	   }
	   default:
	       return DefWindowProc(hWnd, umessage, wparam, lparam);
	}
}

DisplayWin32::DisplayWin32(LPCWSTR applicationName, Game* game, int width, int height)
    : ClientWidth(width), ClientHeight(height)
{
    hInstance = GetModuleHandle(nullptr);

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = &WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = applicationName;
    wc.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Failed to register window class", L"Error", MB_OK);
        return;
    }

    RECT windowRect = { 0, 0, static_cast<LONG>(ClientWidth), static_cast<LONG>(ClientHeight) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

    auto posX = (GetSystemMetrics(SM_CXSCREEN) - ClientWidth) / 2;
    auto posY = (GetSystemMetrics(SM_CYSCREEN) - ClientHeight) / 2;

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName,
                          dwStyle,
                          posX, posY,
                          windowRect.right - windowRect.left,
                          windowRect.bottom - windowRect.top,
                          nullptr, nullptr, hInstance, nullptr);
                          
                          

    if (!hWnd)
    {
        MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK);
        exit(-1);
    }
    
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)game);

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    ShowCursor(true);
}

DisplayWin32::~DisplayWin32()
{
    if (hWnd)
    {
        DestroyWindow(hWnd);
        hWnd = nullptr;
    }
    UnregisterClass(wc.lpszClassName, hInstance);
}