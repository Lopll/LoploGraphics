#pragma once

#include "GameComponent.h"
#include "SimpleMath.h"
#include "CameraComponent.h"

using namespace DirectX::SimpleMath;

class OrbitalCameraComponent : public CameraComponent
{
public:
    float distance = 50.f;
    Vector3 lookAtPos;
    
    OrbitalCameraComponent(TransformData& ownerTransform);
    void SetLookAt(Vector3 lookAt) override;
    void Update(float dt) override;
    
};