#include "GameComponent.h"

GameComponent::GameComponent(Game* gamePtr, Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color)
    : game(gamePtr), 
    constantData(ConstantData(Matrix(), Color)), 
    transform(TransformData(
        Matrix::CreateScale(Scale),
        Matrix::CreateRotationZ(DirectX::XMConvertToRadians(Rotation)), 
        Matrix::CreateTranslation(Translation)))
{}

void GameComponent::ApplyTransform(TransformData newTransform)
{
    constantData.Transform = (ProjectionMatrix * newTransform.Scale * newTransform.Rotation * newTransform.Translation).Transpose();
}