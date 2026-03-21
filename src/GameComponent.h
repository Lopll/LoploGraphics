#pragma once

#include "FrameResources.h"

using namespace DirectX::SimpleMath;

class Game;

struct TransformData
{
    Vector3 Scale;
    float Rotation;
    Vector3 Translation;
};


class GameComponent
{
public:    
    struct ObjectConstants constantData;
    TransformData& transform;

    Matrix ProjectionMatrix;

    GameComponent
    (
        TransformData& ownerTransform,
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void setProjectionMatrix(Matrix proj) { ProjectionMatrix = proj; }
    virtual void setTranslation(Vector3 translation) { transform.Translation = translation; }
    virtual void ApplyTransform();
    
    virtual void DestroyResources(){};
    virtual void Draw(){};
    virtual void Initialize(){};
    virtual void Reload(){};
    virtual void Update(){};
};