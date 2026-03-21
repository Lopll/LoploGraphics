#pragma once

#include "GameComponent.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <iostream>
#include <array>

using namespace DirectX::SimpleMath;

class TriangleComponent : public GameComponent
{
private:
    Microsoft::WRL::ComPtr <ID3D11InputLayout> layout;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderByteCode;
    Vector4 points[8] = 
    {
		Vector4(-0.25f, 0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.25f, -0.25f, 0.0f, 1.0f),    Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.25f, -0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.25f, 0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)
	};
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rastState;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderByteCode;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr <ID3D11Buffer> indexBuffer;
    std::array<int,3> indices;
protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
public:

    TriangleComponent
    (
        TransformData& ownerTransform,
        std::array<int,3> idx = {0,1,2},
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};