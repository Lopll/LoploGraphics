#include "RectangleComponent.h"

RectangleComponent::RectangleComponent(Game* gamePtr, std::array<int,6> idx, Vector4 Position, Matrix Rotation, Matrix Scale, Vector4 Color):
    GameComponent(gamePtr, Position, Rotation, Scale, Color), 
    triangle1(gamePtr, std::array<int,3> {idx[0], idx[1], idx[2]}, Position, Rotation, Scale, Color), 
    triangle2(gamePtr, std::array<int,3> {idx[3], idx[4], idx[5]}, Position, Rotation, Scale, Color)
{
    
}

void RectangleComponent::Initialize(Matrix projectionMatrix)
{
	triangle1.Initialize(projectionMatrix);
	triangle2.Initialize(projectionMatrix);
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