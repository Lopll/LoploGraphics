#include "GameComponent.h"


GameComponent::GameComponent(Game* gamePtr, DirectX::XMFLOAT4 Position, DirectX::XMFLOAT4 Rotation, DirectX::XMFLOAT4 Scale, DirectX::XMFLOAT4 Color)
    : game(gamePtr), constantData(ConstantData(TransformData (Position, Rotation, Scale), Color))
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