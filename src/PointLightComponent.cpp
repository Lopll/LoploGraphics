#include "PointLightComponent.h"

PointLightComponent::PointLightComponent(TransformData& ownerTransform, Vector3 color, GameComponent* parent, float intencity, float radius)
    : GameComponent(ownerTransform, Vector4(color.x, color.y, color.z, 1.f), parent), Intencity(intencity), Radius(radius), Color(color)
{
    
}