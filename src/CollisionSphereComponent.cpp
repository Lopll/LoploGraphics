#include "CollisionSphereComponent.h"
#include <iostream>

CollisionSphereComponent::CollisionSphereComponent(TransformData& ownerTransform, Vector3 centerOffset, float radius)
    : GameComponent(ownerTransform),
      bounds(centerOffset, radius)
{
}

void CollisionSphereComponent::Update(float dt)
{
    bounds.Center = transform.Translation;
    // bounds.Radius = max(transform.Scale.x,max(transform.Scale.y,transform.Scale.z)) * 0.5f;
}

bool CollisionSphereComponent::Intersects(const CollisionSphereComponent& other) const
{
    return bounds.Intersects(other.bounds);
}

DirectX::ContainmentType CollisionSphereComponent::Contains(const CollisionSphereComponent& other) const
{
    return bounds.Contains(other.bounds);
}