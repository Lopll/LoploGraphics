#include "Pong.h"

//TODO:: 

#include <array>
#include <iostream>
#include "RectangleComponent.h"
int width = 640;
int height = 360;
int gridStep = 50;

Pong::Pong():
    Game(L"LoploPong", width, height)
{

    for(int i = gridStep; i <= height*2; i+=gridStep)
    {
        // std::cout << "Creating grid unit on y = " << 1.0f -((float)i/height) << std::endl;
        Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector4(0.0f, 1.0f - ((float)i/height) , 0.0f, 0.0f), Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0.0f)), Matrix::CreateScale(1.0f, 1.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
    }

}

void Pong::UpdateInput()
{
    
}