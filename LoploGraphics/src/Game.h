// TODO: all com pointers?
// MEGA TODO: 
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
    DisplayWin32 Display;
    InputDevice Input;
    std::vector<std::unique_ptr<GameComponent>> Components;
    
    // DebugAnnotation; 
    
    static Game* Instance;

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
    
    bool ScreenResized = false;// find method for it. Maybe bool value, which will be wathed somewhere for resize

    // ID3D11Buffer* constantBuffer;
    
    Game(LPCWSTR Name = L"Waht", int width = 800, int height = 800);
    
    void Exit();
    bool MessageHandler();
    void RestoreTargets();
    int Run();
    Matrix CalcProjectionMatrix();
    void Resize();
    
    
    static void DestroyResources();
    static void Draw();
    static void EndFrame();
    static bool Initialize();
    static void SetViewport();
    static void PrepareFrame();
    static void PrepareResources();
    static void Update();
    static void UpdateInput();
    static void UpdateInternal();

private:
    void CreateBackBuffer();
};