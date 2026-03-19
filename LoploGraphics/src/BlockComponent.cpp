#include "BlockComponent.h"

BlockComponent::BlockComponent(Game* gamePtr, std::array<int,6> idx, Vector3 Translation, float Rotation, Vector3 Scale, Vector4 Color):
    GameComponent(gamePtr, Translation, Rotation, Scale, Color), 
    form(gamePtr, idx, Translation, Rotation, Scale, Color)
{
    
}

void BlockComponent::setProjectionMatrix(Matrix proj)
{
    ProjectionMatrix = proj;
    form.setProjectionMatrix(proj);
}

void BlockComponent::Initialize()
{
    form.Initialize();
}

void BlockComponent::Draw()
{
    form.Draw();
}

void BlockComponent::Update()
{
    form.Update();
}

void BlockComponent::DestroyResources()
{
    form.DestroyResources();
}

void BlockComponent::setTranslation(Matrix translation) 
{ 
	transform.Translation = translation;
	form.setTranslation(translation);
}
