#include "GameComponent.h"

GameComponent::GameComponent(Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color)
    : constantData(ObjectConstants(Matrix(), Color)), 
    transform(TransformData(Scale, Rotation, Translation))
{}

void GameComponent::ApplyTransform(TransformData newTransform)
{
    constantData.World = ProjectionMatrix
                            * Matrix::CreateScale(newTransform.Scale)
                            * Matrix::CreateRotationZ(DirectX::XMConvertToRadians(newTransform.Rotation))
                            * Matrix::CreateTranslation(newTransform.Translation);
    constantData.World = constantData.World.Transpose();
}