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
#include <unordered_map>

#include "DisplayWin32.h"
#include "InputDevice.h"
#include "GameComponent.h"
#include "CameraComponent.h"
#include "OrbitalCameraComponent.h"
#include "RenderComponent.h"
#include "RectangleComponent.h"
#include "GeometryGenerator.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "tiny_obj_loader.h"

using namespace DirectX::SimpleMath;

typedef struct Entity {
    int id = 0;
    TransformData transform;
    std::unordered_map<std::string, GameComponent*> components;

    template<typename T, typename... Args>
    T* AddComponent(const std::string& name, Args&&... args)
    {
        // static_assert(std::is_base_of_v<GameComponent, T>,
            // "T must derive from GameComponent!");

        auto newComp = std::make_unique<T>(transform, std::forward<Args>(args)...);

        T* rawPtr = newComp.get();
        components[name] = rawPtr;

        Game::Instance->Components.push_back(std::move(newComp));
        return rawPtr;
    }

    template<typename T>
    T* GetComponent(const std::string& name)
    {
        auto it = components.find(name);
        if (it == components.end())
            return nullptr;

        return dynamic_cast<T*>(it->second);
    }

} Entity;

struct LightSource
{
	Vector3 Direction;
	float Intencity;
	LightSource(Vector3 dir = Vector3(0.f), float i = 1.f): Direction(dir), Intencity(i) {};
};

class Game
{
public:
    DisplayWin32 Display;
    
    struct LightSource directionalLight;
    Microsoft::WRL::ComPtr<ID3D11Buffer> LightBuffer;
    
    CameraComponent* Camera;
    float fov = 103.f;
    float aspectRatio;
    float nearZ = 0.1f;
    float farZ = 10000.f;
    
    InputDevice Input;
    std::vector<std::unique_ptr<GameComponent>> Components;
    std::unordered_map<std::string, Entity> Entities;
    
    static Game* Instance;
    
    bool wireframe = false;
    
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
    Microsoft::WRL::ComPtr<ID3D11Buffer> ProjectionBuffer;
    
    Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthView;
    
    Microsoft::WRL::ComPtr<ID3D11Texture2D> ShadowMap;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> ShadowMapDepthView;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShadowMapResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> ShadowSampler;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> ShadowRS;
    
    bool ScreenResized = false;

    Game(LPCWSTR Name = L"Waht", int width = 800, int height = 800);

    void Exit();
    bool MessageHandler();
    void RestoreTargets();
    virtual int Run();
    Matrix CalcProjectionMatrix();
    void Resize();
    void UpdateProjectionBuffer(Matrix proj, Matrix view);

    void DestroyResources();
    virtual void Draw();
    virtual void DrawShadow();
    void EndFrame();
    virtual bool Initialize();
    void SetViewport(float w, float h);
    void PrepareFrame();
    void PrepareShadowFrame();
    virtual void PrepareResources();
    virtual void Update(float dt);
    virtual void UpdateInput();
    virtual void UpdateLight();
    float  UpdateInternal();

    void InitImGui();
    void ShutdownImGui();
    void NewImGuiFrame();
    void RenderImGui();
    
    bool LoadOBJModel(const std::string filepath, std::vector<Vertex>& outVertices, std::vector<std::uint32_t>& outIndices, Vector4 defaultColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    void RemoveComponentFromEntity(const std::string& entityName, const std::string& componentName);
private:
    void CreateBackBuffer();
};