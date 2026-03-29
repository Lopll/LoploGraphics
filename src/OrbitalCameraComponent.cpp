#include "OrbitalCameraComponent.h"
OrbitalCameraComponent::OrbitalCameraComponent(TransformData& ownerTransform): CameraComponent(ownerTransform)
{
    
}

void OrbitalCameraComponent::SetLookAt(Vector3 lookAt)
{
    lookAtPos = lookAt;
}

void OrbitalCameraComponent::Update(float dt)
{
    Vector3 camPos = Vector3(distance, 0.f, 0.f);
    
    Matrix rotMat = Matrix::CreateFromYawPitchRoll(transform.Rotation.x, 0.f, transform.Rotation.z);
    camPos = Vector3::Transform(camPos, rotMat) + lookAtPos;
    view = Matrix::CreateLookAt(camPos, lookAtPos, Vector3::Up);
}

void OrbitalCameraComponent::AdjustPosition(Vector3 adjustment)
{
    distance += adjustment.z;
}

void OrbitalCameraComponent::AdjustRotation(Vector3 adjustment)
{
    if(std::abs(transform.Rotation.z + adjustment.z) <= 1.49f)
    {
        transform.Rotation += adjustment;
    }
}