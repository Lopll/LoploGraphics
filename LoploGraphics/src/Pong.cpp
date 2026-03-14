#include "Pong.h"

//TODO:: прямоугольники не видно. Нужно определить методы другие для отрисовки.

#include <array>
#include "RectangleComponent.h"

Pong::Pong():
    Game(L"LoploPong")
{
    // Components.push_back(std::make_unique<TriangleComponent>(this, std::array<int, 3>{0,1,2}, Matrix::CreateTranslation(0.0f, 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
    
    // Components.push_back(std::make_unique<TriangleComponent>(this, std::array<int, 3>{0,3,2}, Matrix::CreateTranslation(0.0f, 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
    Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Matrix::CreateTranslation(0.0f, 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(1.0f, 1.0f, 1.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
}