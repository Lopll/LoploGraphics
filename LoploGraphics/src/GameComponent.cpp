#include "GameComponent.h"

GameComponent::GameComponent(Game* gamePtr, Matrix Position, Matrix Rotation, Matrix Scale, Vector4 Color)
    : game(gamePtr), 
    constantData(ConstantData(Matrix(), Color)), 
    transform(TransformData(Position, Rotation, Scale))
{
        
}

void GameComponent::DestroyResources()
{

}

void GameComponent::Draw()
{

}

void GameComponent::Initialize()
{

}

void GameComponent::Update()
{
    
}

void GameComponent::Reload()
{

}

void GameComponent::ApplyTransform()
{
    constantData.Transform = (transform.Scale * transform.Rotation * transform.Position).Transpose(); 
}