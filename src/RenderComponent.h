#pragma once

#include "GameComponent.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#include <DDSTextureLoader.h>

// TODO: add ambient, diffuse and specular vectors

struct Vertex
{
    DirectX::SimpleMath::Vector3 Pos;
    DirectX::SimpleMath::Vector4 Color = {1,1,1,1};
    DirectX::SimpleMath::Vector2 TexCoord;
    DirectX::SimpleMath::Vector3 Normal;
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
    
    const wchar_t* diffuseFilename;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture;
    const wchar_t* aoFilename;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> aoTexture;
    const wchar_t* normalFilename;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalTexture;
    const wchar_t* roughFilename;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughTexture;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
#pragma endregion 
    std::vector<std::uint32_t> indices;

    void LoadShadersAndLayout();
    void LoadGeometry();

public:
    bool wireframe = false;
    std::vector<Vertex> vertices;
    
    
    
    RenderComponent
    (
        TransformData& ownerTransform,
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        GameComponent* parent = nullptr,
        std::vector<Vertex> Vertices = {{ Vector3(-0.5f, 0.5f, 0.0f),  Color(1, 0, 0), Vector2(0,0) },
                                        { Vector3(-0.5f, -0.5f, 0.0f), Color(1, 1, 1), Vector2(0,0) },
                                        { Vector3(0.5f, -0.5f, 0.0f),  Color(0, 0, 1), Vector2(0,0) }},
        std::vector<std::uint32_t> Indices = { 0, 1, 2 },
        const wchar_t* diffuse = L""
    );
    
    float CalculateBoundingRadius();

    void DestroyResources() override;
    void Draw() override;
    void DrawShadow() override;
    void Initialize() override;
    void Update(float dt) override;
};

