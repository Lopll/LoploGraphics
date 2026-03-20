#include "BlockComponent.h"

BlockComponent::BlockComponent(Game* gamePtr, std::array<int,6> idx, Vector3 Scale, float Rotation, Vector3 Translation, Vector4 Color):
    GameComponent(gamePtr, Scale, Rotation, Translation, Color),
    form(gamePtr, idx, Scale, Rotation, Translation, Color)
{
    /*std::vector<Vector3> vert;
    vert.push_back(Vector3(-0.25f, 0.25f, 0.0f));
    vert.push_back(Vector3(-0.25f, -0.25f, 0.0f));
    vert.push_back(Vector3(-0.25f, -0.25f, 0.0f));
    vert.push_back(Vector3(0.25f, 0.25f, 0.0f));
    DirectX::BoundingBox::CreateFromPoints(collision, vert.size(), vert.data(), sizeof(Vector3));*/
     DirectX::BoundingBox::CreateFromPoints(collision, Vector3(-0.25f, -0.25f, 0.f), Vector3(0.25f, 0.25f, 0.f));
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

void BlockComponent::setTranslation(Vector3 translation) 
{ 
	transform.Translation = translation;
	form.setTranslation(translation);
}

void BlockComponent::ApplyTransform(TransformData newTransform)
{
    // auto m = form.triangle1.constantData.Transform.Transpose();
    // DirectX::SimpleMath::Vector3 v1 = DirectX::SimpleMath::Vector3(m._41, m._42, m._43);
    // auto m2 = form.triangle2.constantData.Transform.Transpose();
    // DirectX::SimpleMath::Vector3 v2 = DirectX::SimpleMath::Vector3(m2._41, m2._42, m2._43);
    // DirectX::SimpleMath::Vector3 v = v1 + v2;
    // v /= 2.f;

    //std::cout << "v1: " << v1.x << ' ' << v1.y << ' ' << v1.z << std::endl;
    //std::cout << "v2: " << v2.x << ' ' << v2.y << ' ' << v2.z << std::endl;
    //std::cout << "v: " << v.x << ' ' << v.y << ' ' << v.z << std::endl;

    //collision.Transform(collision, DirectX::SimpleMath::Matrix::CreateTranslation(v));
    
    // Matrix colTra = ProjectionMatrix;
    // colTra *= Matrix::CreateScale(newTransform.Scale);
    // colTra *= Matrix::CreateRotationZ(DirectX::XMConvertToRadians(newTransform.Rotation));
    // colTra *= Matrix::CreateTranslation(newTransform.Translation);
    
    collision.Transform(collision, constantData.Transform.Transpose());
    GameComponent::ApplyTransform(newTransform);

}