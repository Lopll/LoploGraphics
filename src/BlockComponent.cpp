#include "BlockComponent.h"

BlockComponent::BlockComponent(TransformData& ownerTransform, std::array<int,6> idx, Vector4 Color)
    : GameComponent(ownerTransform, Color),
    form(ownerTransform, idx, Color)
{
    
}

void BlockComponent::Initialize()
{
    form.Initialize();
}

void BlockComponent::Draw()
{
    form.Draw();
}

void BlockComponent::Update(float dt)
{
    form.Update(dt);
    ApplyTransform();
}

void BlockComponent::DestroyResources()
{
    form.DestroyResources();
}