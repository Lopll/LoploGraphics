#pragma once

#include <DirectXCollision.h>

#include "GameComponent.h"
#include "RectangleComponent.h"


class BlockComponent : public GameComponent
{
private:
    RectangleComponent form;
public:
    DirectX::BoundingBox collision;
    
    BlockComponent
    (
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Vector3 Scale = Vector3(1.0f),
        float Rotation = 0.0f, 
        Vector3 Translation = Vector3(0.0f), 
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    
    void setProjectionMatrix(Matrix proj) override;
    void setTranslation(Vector3 translation) override;
    
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
    
    void ApplyTransform(TransformData newTransform = TransformData()) override;
};