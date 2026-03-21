#include "RectangleComponent.h"

RectangleComponent::RectangleComponent(std::array<int,6> idx, Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color):
    GameComponent(Scale, Rotation, Translation, Color),
    triangle1(std::array<int,3> {idx[0], idx[1], idx[2]}, Scale, Rotation, Translation, Color),
    triangle2(std::array<int,3> {idx[3], idx[4], idx[5]}, Scale, Rotation, Translation, Color)
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

void RectangleComponent::Update()
{   
    triangle1.transform = transform;
	triangle1.Update();
	triangle2.transform = transform;
	triangle2.Update();
}

void RectangleComponent::DestroyResources()
{
	triangle1.DestroyResources();
	triangle2.DestroyResources();
}

void RectangleComponent::setProjectionMatrix(Matrix proj)
{
	ProjectionMatrix = proj;
	triangle1.setProjectionMatrix(proj);
	triangle2.setProjectionMatrix(proj);
}

void RectangleComponent::setTranslation(Vector3 translation) 
{ 
	transform.Translation = translation;
	triangle1.setTranslation(translation);
	triangle2.setTranslation(translation);
}