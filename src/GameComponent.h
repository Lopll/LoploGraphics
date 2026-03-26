#pragma once

#include "FrameResources.h"

using namespace DirectX::SimpleMath;

class Game;

struct TransformData
{
    Vector3 Scale       = {1, 1, 1};
    Vector3 Rotation    = {0, 0, 0};
    Vector3 Translation = {0, 0, 0};
};

class GameComponent
{
public:    
    struct ObjectConstants constantData;
    TransformData& transform;

    GameComponent
    (
        TransformData& ownerTransform,
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void ApplyTransform();
    
    virtual void DestroyResources(){};
    virtual void Draw(){};
    virtual void Initialize(){};
    virtual void Reload(){};
    virtual void Update(float dt){};
};