// TODO: all com pointers?

// MEGA TODO: найти у Луны инфу про offset и Transform, решить вопрос, где лучше считать каждый компонент transform: в шейдере или в коде игры (на видеокарте, может, быстрее будет?)
#pragma once

#include <windows.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>
#include <memory>
#include <vector>

#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "GameComponent.h"
#include "TriangleComponent.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class Game
{
public:

    LPCWSTR Name = L"Waht";

    DisplayWin32 Display;
    InputDevice Input;
    std::vector<std::unique_ptr<GameComponent>> Components;
    
    // DebugAnnotation; no in super
    
    static Game* Instance; // Get access to non-static fields from static methods

    // FPS counter
    std::chrono::time_point<std::chrono::steady_clock> PrevTime;
    std::chrono::time_point<std::chrono::steady_clock> StartTime;
    float TotalTime;

    Microsoft::WRL::ComPtr<ID3D11Device> Device;
    ID3D11Texture2D* backBuffer;
    ID3D11DeviceContext* Context;
    // ID3D11ShaderResourceView* RenderSRV;
    ID3D11RenderTargetView* RenderView;
    IDXGISwapChain* SwapChain;
    
    // ScreenResized; find method for it. Maybe bool value, which will be wathed somewhere for resize

    // ID3D11Buffer* constantBuffer;
    
    Game();
    
    void Exit();
    bool MessageHandler();
    void RestoreTargets();
    int Run();
    
    void static DestroyResources();
    void static Draw();
    void static EndFrame();
    bool static Initialize();
    void static PrepareFrame();
    void static PrepareResources();
    void static Update();
    void static UpdateInput();
    void static UpdateInternal();

private:
    void CreateBackBuffer();
};