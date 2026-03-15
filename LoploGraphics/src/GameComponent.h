#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>
#include <d3d11.h>

using namespace DirectX::SimpleMath;

class Game;

struct TransformData
{
    Matrix Translation;
    Matrix Rotation;
    Matrix Scale;
};

struct ConstantData
{
    Matrix Transform;
    Vector4 ActualPosition;
    Vector4 Color;
};

class GameComponent
{
public:
    Game* game;
    
    ID3D11Buffer* constantBuffer;
    struct ConstantData constantData;
    
    struct TransformData transform;    
    
    GameComponent
    (
        Game* gamePtr = nullptr, 
        Vector4 Position = Vector4(0.0f), 
        Matrix Rotation = Matrix(), 
        Matrix Scale = Matrix::CreateScale(1.0f, 1.0f, 1.0f),
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void DestroyResources();
    virtual void Draw();
    virtual void Initialize();
    virtual void Reload();
    virtual void Update();
    
    void ApplyTransform(TransformData newTransform = TransformData());
};