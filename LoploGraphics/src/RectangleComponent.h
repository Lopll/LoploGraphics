#pragma once

#include "GameComponent.h"
#include "TriangleComponent.h"

class RectangleComponent : public GameComponent
{
private:
    TriangleComponent triangle1;
    TriangleComponent triangle2;
public:
    RectangleComponent
    (
        Game* gamePtr = nullptr,
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Matrix Position = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f), 
        Matrix Rotation = Matrix(), 
        Matrix Scale = Matrix::CreateScale(1.0f, 1.0f, 1.0f),
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
}; 