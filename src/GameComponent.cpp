#include "GameComponent.h"

GameComponent::GameComponent(Game* gamePtr, Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color)
    : game(gamePtr), 
    constantData(ConstantData(Matrix(), Color)), 
    transform(TransformData(Scale, Rotation, Translation))
{}

void GameComponent::ApplyTransform(TransformData newTransform)
{
    constantData.Transform = ProjectionMatrix;
    constantData.Transform *= Matrix::CreateScale(newTransform.Scale);
    constantData.Transform *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(newTransform.Rotation));
    constantData.Transform *= Matrix::CreateTranslation(newTransform.Translation);
    constantData.Transform = constantData.Transform.Transpose();
}