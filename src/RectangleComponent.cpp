#include "RectangleComponent.h"

RectangleComponent::RectangleComponent(TransformData& ownerTransform, std::array<int,6> idx, Vector4 Color):
    GameComponent(ownerTransform, Color),
    triangle1(ownerTransform, std::array<int,3> {idx[0], idx[1], idx[2]}, Color),
    triangle2(ownerTransform, std::array<int,3> {idx[3], idx[4], idx[5]}, Color)
{
    
}

void RectangleComponent::Initialize()
{
	triangle1.Initialize();
	triangle2.Initialize();
}

void RectangleComponent::Draw()
{
	triangle1.Draw();
	triangle2.Draw();
}

void RectangleComponent::Update(float dt)
{   
	triangle1.Update(dt);
	triangle2.Update(dt);
}

void RectangleComponent::DestroyResources()
{
	triangle1.DestroyResources();
	triangle2.DestroyResources();
}
