#pragma once

#include "GameComponent.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

struct Vertex
{
    DirectX::SimpleMath::Vector3 Pos;
    DirectX::SimpleMath::Vector4 Color = {1,1,1,1};
};

class RenderComponent : public GameComponent
{
protected:
#pragma region DirectX Resources
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderByteCode;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeState;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderByteCode;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
#pragma endregion 
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    void LoadShadersAndLayout();
    void LoadGeometry();

public:
    bool wireframe = false;
    RenderComponent
    (
        TransformData& ownerTransform,
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        GameComponent* parent = nullptr,
        std::vector<Vertex> Vertices = {{ Vector3(-0.5f, 0.5f, 0.0f),  Color(1, 0, 0) },
                                        { Vector3(-0.5f, -0.5f, 0.0f), Color(1, 1, 1) },
                                        { Vector3(0.5f, -0.5f, 0.0f),  Color(0, 0, 1) }},
        std::vector<std::uint32_t> Indices = { 0, 1, 2 }
    );

    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update(float dt) override;
};

