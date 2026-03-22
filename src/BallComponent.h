#pragma once

#include "GameComponent.h"
#include "RectangleComponent.h"
#include "CollisionComponent.h"
#include <SimpleMath.h>
#include <DirectXCollision.h>
#include <algorithm>
#include <cmath>

using namespace DirectX::SimpleMath;

class BallComponent : public GameComponent
{
private:
    RectangleComponent form;
public:
    Vector3 movementDirection;    
    float movementSpeed;
    float movementAcceleration;
    const float MAX_SPEED = 30.f;
    
    BallComponent
    (
        TransformData& ownerTransform,
        Vector3 dir = Vector3(1.f, 0.f, 0.f),
        float speed = 3.f,
        float accel = 1.1f,
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
       
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;

    void OnBoundsContact(CollisionComponent* ball, CollisionComponent* box);
    void OnPlayerContact(CollisionComponent* ball, CollisionComponent* box);
};
