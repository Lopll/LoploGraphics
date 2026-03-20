#pragma once

#include "GameComponent.h"
#include "TriangleComponent.h"

class RectangleComponent : public GameComponent
{
private:
    
public:
    TriangleComponent triangle1;
    TriangleComponent triangle2;
    RectangleComponent
    (
        Game* gamePtr = nullptr,
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Vector3 Scale = Vector3(1.0f),
        float Rotation = 0.0f, 
        Vector3 Translation = Vector3(0.0f), 
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void setProjectionMatrix(Matrix proj) override;
    void setTranslation(Matrix translation) override;
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};