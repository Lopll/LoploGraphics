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
    DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
    ID3D11RasterizerState* rastState;
    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;
    ID3D11Buffer* vertexBuffer;
    int indices[6] = { 0,1,2, 1,0,3 };
    ID3D11Buffer* indexBuffer;
public:
    TriangleComponent(Game* gamePtr = nullptr);
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};