#include "GameComponent.h"

GameComponent::GameComponent(Game* gamePtr, Vector4 Position, Matrix Rotation, Matrix Scale, Vector4 Color)
    : game(gamePtr), 
    constantData(ConstantData(Matrix(), Position, Color)), 
    transform(TransformData(Matrix::CreateTranslation(0.0f, 0.0f, 0.0f), Rotation, Scale))
{
        
}

void GameComponent::DestroyResources()
{

}

void GameComponent::Draw()
{

}

void GameComponent::Initialize(Matrix projectionMatrix)
{
    ProjectionMatrix = projectionMatrix;
}

void GameComponent::Update()
{
    
}

void GameComponent::Reload()
{

}

void GameComponent::ApplyTransform(TransformData newTransform)
{
    Matrix Empty{};
    if (newTransform.Translation == Empty and newTransform.Rotation == Empty and newTransform.Scale == Empty)
    {
        newTransform = transform;
    }
    
    constantData.Transform = (newTransform.Scale * newTransform.Rotation * newTransform.Translation * ProjectionMatrix).Transpose();
}