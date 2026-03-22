#include "BallComponent.h"

#include <iostream>

BallComponent::BallComponent(TransformData& ownerTransform, Vector3 dir, float speed, float accel, std::array<int,6> idx, Vector4 Color)
    : GameComponent(ownerTransform, Color),
    form(ownerTransform, idx, Color),
    movementDirection(dir),
    movementSpeed(speed),
    movementAcceleration(accel)
{

}


void BallComponent::Initialize()
{
    form.Initialize();
}

void BallComponent::Draw()
{
    form.Draw();
}

void BallComponent::Update(float dt)
{
    form.Update(dt);
    ApplyTransform();
    
    // move to dir
    transform.Translation += movementDirection * movementSpeed * dt;
}

void BallComponent::DestroyResources()
{
    form.DestroyResources();
}

void BallComponent::OnBoundsContact(CollisionComponent* ball, CollisionComponent* box)
{
    movementDirection = Vector3::Reflect(movementDirection, -ball->calcNormal(box->bounds));
    movementDirection.Normalize(); 
}

void BallComponent::OnPlayerContact(CollisionComponent* ball, CollisionComponent* box)
{
    movementSpeed = (std::min)(movementSpeed * movementAcceleration, MAX_SPEED);
    
    Vector3 refl = box->bounds.Center - ball->bounds.Center;
    // refl.y *= -1;
    
    movementDirection = Vector3::Reflect(refl, ball->calcNormal(box->bounds));
    movementDirection.Normalize(); 
    
    // std::cout << movementDirection.x << " " << movementDirection.y << std::endl << std::endl;
}