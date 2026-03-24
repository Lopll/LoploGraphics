#pragma once
#include <SimpleMath.h>
#include "GameComponent.h"

using namespace DirectX::SimpleMath;

class CameraComponent : public GameComponent
{
public:
    Matrix view;
    Matrix projection;
    
    CameraComponent(TransformData& ownerTransform);
    void SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ);
    void SetLookAt(Vector3 lookAt);
    void Update(float dt) override;
    
};