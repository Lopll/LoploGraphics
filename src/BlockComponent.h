#pragma once

#include <DirectXCollision.h>

#include "GameComponent.h"
#include "RectangleComponent.h"


class BlockComponent : public GameComponent
{
private:
    RectangleComponent form;

public:    
    BlockComponent
    (
        TransformData& ownerTransform,
        std::array<int,6> idx = {0,1,2, 0,3,2},
        Vector4 Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
    );
        
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update(float dt) override;
};