#include "GameComponent.h"

GameComponent::GameComponent(Game* gamePtr, Vector3 Translation, float Rotation, Vector3 Scale, Vector4 Color)
    : game(gamePtr), 
    constantData(ConstantData(Matrix(), Color)), 
    transform(TransformData(
        Matrix::CreateTranslation(Translation), 
        Matrix::CreateRotationZ(DirectX::XMConvertToRadians(Rotation)), 
        Matrix::CreateScale(Scale)))
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

void GameComponent::ApplyTransform(TransformData newTransform)
{
    constantData.Transform = (ProjectionMatrix * newTransform.Scale * newTransform.Rotation * newTransform.Translation).Transpose();
    // constantData.Transform = (ProjectionMatrix * newTransform.Translation * newTransform.Rotation * newTransform.Scale).Transpose();
}