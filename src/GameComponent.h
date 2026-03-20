#pragma once

#include "FrameResources.h"
#include <wrl/client.h>

using namespace DirectX::SimpleMath;

class Game;

struct TransformData
{
    Matrix Scale;
    Matrix Rotation;
    Matrix Translation;
};


class GameComponent
{
public:
    Game* game;
    
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    struct ConstantData constantData;
    struct TransformData transform;    
    
    Matrix ProjectionMatrix;

    GameComponent
    (
        Game* gamePtr = nullptr, 
        Vector3 Scale = Vector3(1.0f),
        float Rotation = 0.0f, 
        Vector3 Translation = Vector3(0.0f), 
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void setProjectionMatrix(Matrix proj) { ProjectionMatrix = proj; }
    virtual void setTranslation(Matrix translation) { transform.Translation = translation; }
    virtual void ApplyTransform(TransformData newTransform = TransformData());
    
    virtual void DestroyResources(){};
    virtual void Draw(){};
    virtual void Initialize(){};
    virtual void Reload(){};
    virtual void Update(){};
};