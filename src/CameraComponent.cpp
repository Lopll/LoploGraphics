#include "CameraComponent.h"

CameraComponent::CameraComponent(TransformData& ownerTransform): GameComponent(ownerTransform)
{
    
}

void CameraComponent::SetProjectionValues(float fov, float aspectRatio, float nearZ, float farZ)
{
    projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(fov), aspectRatio, nearZ, farZ);
}

void CameraComponent::SetLookAt(Vector3 lookAt)
{
    if(lookAt.x == transform.Translation.x && lookAt.y == transform.Translation.y && lookAt.z == transform.Translation.z)
    {
        return;
    }
    
    lookAt -= transform.Translation;
    
    if(lookAt.y != 0.f)
    {
        const float d = sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z);
        transform.Rotation.x = atan(lookAt.y / d);
    }
    
    if(lookAt.x != 0.f)
    {
        transform.Rotation.y = atan(lookAt.x / lookAt.z);
    }
    if(lookAt.z > 0)
    {
        transform.Rotation.y += DirectX::XM_PI;
    }
}

void CameraComponent::Update(float dt)
{
    Matrix rot = Matrix::CreateFromYawPitchRoll(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z);
    Vector3 target = Vector3::Transform(Vector3::Forward, rot);
    target += transform.Translation;
    Vector3 upDir = Vector3::Transform(Vector3::Up, rot);
    view = Matrix::CreateLookAt(transform.Translation, target, upDir);
    
    
    // Vector3 camPos = Vector3(50.f, 0.f, 0.f);
    
    // Matrix rotMat = Matrix::CreateFromYawPitchRoll(45.f, 0.f, 0.f);
    // view = Matrix::CreateLookAt(camPos, Vector3(), Vector3::Up);
    // projection = Matrix::CreatePerspectiveFieldOfView(90.f, 1.f, 0.1f, 1000.f);
}

void CameraComponent::AdjustPosition(Vector3 adjustment)
{
    adjustment = Vector3::Transform(adjustment, Matrix::CreateFromYawPitchRoll(transform.Rotation.x,transform.Rotation.y,0.f));
    transform.Translation += adjustment;
}