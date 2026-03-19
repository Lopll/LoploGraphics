#include "RectangleComponent.h"

RectangleComponent::RectangleComponent(Game* gamePtr, std::array<int,6> idx, Vector3 Translation, float Rotation, Vector3 Scale, Vector4 Color):
    GameComponent(gamePtr, Translation, Rotation, Scale, Color), 
    triangle1(gamePtr, std::array<int,3> {idx[0], idx[1], idx[2]}, Translation, Rotation, Scale, Color), 
    triangle2(gamePtr, std::array<int,3> {idx[3], idx[4], idx[5]}, Translation, Rotation, Scale, Color)
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

void RectangleComponent::setTranslation(Matrix translation) 
{ 
	transform.Translation = translation;
	triangle1.setTranslation(translation);
	triangle2.setTranslation(translation);
}