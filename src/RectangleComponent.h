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
        TransformData& ownerTransform,
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void setTranslation(Vector3 translation) override;
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
};