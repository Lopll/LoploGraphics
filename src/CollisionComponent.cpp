#include "CollisionComponent.h"
#include <iostream>

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

Vector3 CollisionComponent::calcNormal(DirectX::BoundingBox box)
{
    using namespace std;
    
    Vector3 d = bounds.Center - box.Center;
    Vector3 overlap = (bounds.Extents + box.Extents) - Vector3(abs(d.x), abs(d.y), abs(d.z));
    
    Vector3 normal;
    
    if(overlap.x <= overlap.y && overlap.x)
    {
        float sign = d.x > 0 ? 1.0f : -1.0f;
        normal = Vector3(sign, 0, 0);
    }
    else if(overlap.y <= overlap.x)
    {
        float sign = d.y > 0 ? 1.0f : -1.0f;
        normal = Vector3(0, sign, 0);
    }

    
    return normal;
}