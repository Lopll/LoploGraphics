#include "GameComponent.h"

GameComponent::GameComponent(TransformData& ownerTransform, Vector4 Color, GameComponent* parent)
    : constantData(ObjectConstants(Matrix(), Color)), 
    transform(ownerTransform),
    Parent(parent)
{
}

void GameComponent::ApplyTransform()
{
    constantData.World = Matrix::CreateScale(transform.Scale) * Matrix::CreateFromYawPitchRoll(transform.Rotation) * Matrix::CreateTranslation(transform.Translation);
    
    if(Parent != nullptr)
    {
        constantData.World *= Parent->constantData.World.Transpose();//Matrix::CreateScale(Parent->transform.Scale) * Matrix::CreateFromYawPitchRoll(Parent->transform.Rotation) * Matrix::CreateTranslation(Parent->transform.Translation); 
        
        // constantData.World = Matrix::CreateScale(transform.Scale) * Matrix::CreateScale(Parent->transform.Scale) * Matrix::CreateFromYawPitchRoll(transform.Rotation) * Matrix::CreateFromYawPitchRoll(Parent->transform.Rotation) * Matrix::CreateTranslation(transform.Translation) * Matrix::CreateTranslation(Parent->transform.Translation);
    }
    
    constantData.World = constantData.World.Transpose();
}