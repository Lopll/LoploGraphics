#include "PointLightComponent.h"

PointLightComponent::PointLightComponent(TransformData& ownerTransform, Vector4 Color, GameComponent* parent, float intencity, float radius)
    : GameComponent(ownerTransform, Color, parent), Intencity(intencity), Radius(radius)
{
    
}