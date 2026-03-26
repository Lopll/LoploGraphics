#pragma once
#include <SimpleMath.h>
#include "GameComponent.h"
#include <DirectXMath.h>

using namespace DirectX::SimpleMath;

class CameraComponent : public GameComponent
{
public:
    Matrix view;
    Matrix projection;
    
    CameraComponent(TransformData& ownerTransform);
    void SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ);
    virtual void SetLookAt(Vector3 lookAt);
    void Update(float dt) override;
    
    void AdjustPosition(Vector3 adjustment = Vector3());
    
};