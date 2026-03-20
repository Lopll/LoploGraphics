// TODO: all com pointers?
// MEGA TODO: 
#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <SimpleMath.h>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

#include <iostream>
#include <chrono>
#include <memory>
#include <vector>

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "GameComponent.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

using namespace DirectX::SimpleMath;

class Game
{
public:
    DisplayWin32 Display;
    float aspectRatio;
    
    InputDevice Input;
    std::vector<std::unique_ptr<GameComponent>> Components;
    
    // DebugAnnotation; 
    
    static Game* Instance;
    
    // FPS counter
    std::chrono::time_point<std::chrono::steady_clock> PrevTime;
    std::chrono::time_point<std::chrono::steady_clock> StartTime;
    float TotalTime;
    unsigned int frameCount = 0;

    Microsoft::WRL::ComPtr<ID3D11Device> Device;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderView;
    Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
    // ID3D11ShaderResourceView* RenderSRV;
    
    bool ScreenResized = false;// find method for it. Maybe bool value, which will be wathed somewhere for resize

    // ID3D11Buffer* constantBuffer;
    
    Game(LPCWSTR Name = L"Waht", int width = 800, int height = 800);

    void Exit();
    bool MessageHandler();
    void RestoreTargets();
    virtual int Run();
    Matrix CalcProjectionMatrix();
    void Resize();

    void DestroyResources();
    virtual void Draw();
    void EndFrame();
    virtual bool Initialize();
    void SetViewport();
    void PrepareFrame();
    virtual void PrepareResources();
    virtual void Update();
    virtual void UpdateInput();
    void UpdateInternal();

    void InitImGui();
    void ShutdownImGui();
    void NewImGuiFrame();
    void RenderImGui();

private:
    void CreateBackBuffer();
};