#pragma once

#include "GameComponent.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>

class TriangleComponent : public GameComponent
{
private:
    ID3D11InputLayout* layout;
    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;
    DirectX::XMFLOAT4 points[6] = 
    {
		DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};
    ID3D11RasterizerState* rastState;
    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;
    ID3D11Buffer* vertexBuffer;
    int indices[3] = {0,1,2};
    ID3D11Buffer* indexBuffer;
public:

    TriangleComponent
    (
        Game* gamePtr = nullptr, 
        DirectX::XMFLOAT4 Position = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 
        DirectX::XMFLOAT4 Rotation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 
        DirectX::XMFLOAT4 Scale = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};