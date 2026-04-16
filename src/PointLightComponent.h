#pragma once
#include "GameComponent.h"

class PointLightComponent: public GameComponent
{
public:
    float Intencity;
    float Radius;

    PointLightComponent
    (
        TransformData& ownerTransform,
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        GameComponent* parent = nullptr,
        float intencity = 1.0f,
        float radius = 1.0f
    );
    
      
};