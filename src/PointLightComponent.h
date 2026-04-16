#pragma once
#include "GameComponent.h"

class PointLightComponent: public GameComponent
{
public:
    float Intencity;
    float Radius;
    Vector3 Color;
    
    PointLightComponent
    (
        TransformData& ownerTransform,
        Vector3 color = Vector3(1.0f, 1.0f, 1.0f),
        GameComponent* parent = nullptr,
        float intencity = 1.0f,
        float radius = 1.0f
    );
    
    void Update(float dt) override;
      
};