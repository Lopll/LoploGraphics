#pragma once

#include "GameComponent.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <array>

using namespace DirectX::SimpleMath;

class TriangleComponent : public GameComponent
{
private:
    ID3D11InputLayout* layout;
    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;
    Vector4 points[8] = 
    {
		Vector4(-0.25f, 0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(-0.25f, -0.25f, 0.0f, 1.0f),    Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.25f, -0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4(0.25f, 0.25f, 0.0f, 1.0f),	Vector4(1.0f, 1.0f, 1.0f, 1.0f)
	};
    ID3D11RasterizerState* rastState;
    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;
    ID3D11Buffer* vertexBuffer;
    std::array<int,3> indices;
    ID3D11Buffer* indexBuffer;
protected:
    
public:

    TriangleComponent
    (
        Game* gamePtr = nullptr,
        std::array<int,3> idx = {0,1,2},
        Vector3 Scale = Vector3(1.0f),
        float Rotation = 0.0f, 
        Vector3 Translation = Vector3(0.0f), 
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};