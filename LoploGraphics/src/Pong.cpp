#include "Pong.h"

//TODO:: 

#include <array>
#include "RectangleComponent.h"

Pong::Pong():
    Game(L"LoploPong")
{
    Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector4(-1.0f, -1.0f, 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(0.1f, 0.1f, 1.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f)));
    Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector4(-1.0f, -0.8f, 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(0.1f, 0.1f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));

}

void Pong::UpdateInput()
{
    
}