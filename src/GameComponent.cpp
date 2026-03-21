#include "GameComponent.h"

GameComponent::GameComponent(TransformData& ownerTransform, Vector4 Color)
    : constantData(ObjectConstants(Matrix(), Color)), 
    transform(ownerTransform)
{
}

void GameComponent::ApplyTransform()
{
    constantData.World = Matrix::CreateScale(transform.Scale)
                            * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(transform.Rotation))
                            * Matrix::CreateTranslation(transform.Translation);
    constantData.World = constantData.World.Transpose();
}