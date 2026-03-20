#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

struct ConstantData
{
    DirectX::SimpleMath::Matrix Transform;
    DirectX::SimpleMath::Vector4 Color;
};