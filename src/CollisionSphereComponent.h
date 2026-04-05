#pragma once
#include "GameComponent.h"
#include <DirectXCollision.h>
#include <SimpleMath.h>
#include <cmath>

using namespace DirectX::SimpleMath;

class CollisionSphereComponent : public GameComponent
{
public:
    DirectX::BoundingSphere bounds;

    CollisionSphereComponent(
        TransformData& ownerTransform,
        Vector3 centerOffset = Vector3(0.0f, 0.0f, 0.0f),
        float raduis = 1.f
    );

    void Update(float dt) override;

    bool Intersects(const CollisionSphereComponent& other) const;
    DirectX::ContainmentType Contains(const CollisionSphereComponent& other) const;
};