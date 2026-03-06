#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

class Game;

struct TransformData
{
    DirectX::XMFLOAT4 Position;
    DirectX::XMFLOAT4 Rotation;
    DirectX::XMFLOAT4 Scale;
};

struct ConstantData
{
    struct TransformData Transform;
    DirectX::XMFLOAT4 Color;
};

class GameComponent
{
public:
    Game* game;
    
    ID3D11Buffer* constantBuffer;
    struct ConstantData constantData;
    
    GameComponent
    (
        Game* gamePtr = nullptr, 
        DirectX::XMFLOAT4 Position = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 
        DirectX::XMFLOAT4 Rotation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), 
        DirectX::XMFLOAT4 Scale = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
        DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void DestroyResources();
    virtual void Draw();
    virtual void Initialize();
    virtual void Reload();
    virtual void Update();
};