#include "CollisionComponent.h"
#include "CollisionComponent.h"

CollisionComponent::CollisionComponent(TransformData& ownerTransform,
    Vector3 centerOffset,
    Vector3 extents)
    : GameComponent(ownerTransform),
      bounds(centerOffset, extents)
{
}

void CollisionComponent::Update()
{
    bounds.Center = transform.Translation;
    bounds.Extents = transform.Scale * 0.5f;
}

bool CollisionComponent::Intersects(const CollisionComponent& other) const
{
    return bounds.Intersects(other.bounds);
}

DirectX::ContainmentType CollisionComponent::Contains(const CollisionComponent& other) const
{
    return bounds.Contains(other.bounds);
}