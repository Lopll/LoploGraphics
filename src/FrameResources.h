#pragma once

#include <SimpleMath.h>

struct ObjectConstants
{
    DirectX::SimpleMath::Matrix World                       = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Color Color                        = DirectX::SimpleMath::Color(1, 0, 0);
};

struct PassConstants
{
    DirectX::SimpleMath::Matrix Proj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix InvProj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix View = DirectX::SimpleMath::Matrix::Identity;
};