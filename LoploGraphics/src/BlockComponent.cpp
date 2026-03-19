#include "BlockComponent.h"

BlockComponent::BlockComponent(Game* gamePtr, std::array<int,6> idx, Vector3 Translation, float Rotation, Vector3 Scale, Vector4 Color):
    GameComponent(gamePtr, Translation, Rotation, Scale, Color), 
    form(gamePtr, idx, Translation, Rotation, Scale, Color)
{
    std::vector<Vector3> vert;
    vert.push_back(Vector3(-0.25f, 0.25f, 0.0f));
    vert.push_back(Vector3(-0.25f, -0.25f, 0.0f));
    vert.push_back(Vector3(-0.25f, -0.25f, 0.0f));
    vert.push_back(Vector3(0.25f, 0.25f, 0.0f));
    DirectX::BoundingBox::CreateFromPoints(collision, vert.size(), vert.data(), sizeof(Vector3));
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
    ApplyTransform(transform);
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

void BlockComponent::ApplyTransform(TransformData newTransform)
{
    
    collision.Transform(collision, form.triangle1.constantData.Transform.Transpose());
        GameComponent::ApplyTransform(newTransform);

}