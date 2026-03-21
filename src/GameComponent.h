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
    struct TransformData transform;    
    
    Matrix ProjectionMatrix;

    GameComponent
    (
        Vector3 Scale = Vector3(1.0f),
        float Rotation = 0.0f, 
        Vector3 Translation = Vector3(0.0f), 
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    virtual void setProjectionMatrix(Matrix proj) { ProjectionMatrix = proj; }
    virtual void setTranslation(Vector3 translation) { transform.Translation = translation; }
    virtual void ApplyTransform(TransformData newTransform = TransformData());
    
    virtual void DestroyResources(){};
    virtual void Draw(){};
    virtual void Initialize(){};
    virtual void Reload(){};
    virtual void Update(){};
};