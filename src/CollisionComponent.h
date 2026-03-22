#pragma once
#include "GameComponent.h"
#include <DirectXCollision.h>
#include <SimpleMath.h>
#include <cmath>

using namespace DirectX::SimpleMath;

class CollisionComponent : public GameComponent
{
public:
    DirectX::BoundingBox bounds;

    CollisionComponent(
        TransformData& ownerTransform,
        Vector3 centerOffset = Vector3(0.0f, 0.0f, 0.0f),
        Vector3 extents = Vector3(1.0f, 1.0f, 1.0f)
    );

    void Update() override;

    bool Intersects(const CollisionComponent& other) const;
    DirectX::ContainmentType Contains(const CollisionComponent& other) const;
    
    Vector3 calcNormal(DirectX::BoundingBox box);
};