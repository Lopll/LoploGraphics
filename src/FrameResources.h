#pragma once

#include <SimpleMath.h>

struct ObjectConstants
{
    DirectX::SimpleMath::Matrix World                       = DirectX::SimpleMath::Matrix::Identity;
    //DirectX::SimpleMath::Matrix World                   = DirectX::SimpleMath::Matrix::Identity;
    //DirectX::SimpleMath::Matrix TexTransform            = DirectX::SimpleMath::Matrix::Identity;
    //DirectX::SimpleMath::Matrix InvertTransposeWorld    = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Color Color                        = DirectX::SimpleMath::Color(1, 0, 0);
};

struct PassConstants
{
    DirectX::SimpleMath::Matrix View = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix InvView = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix Proj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix InvProj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix ViewProj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Matrix InvViewProj = DirectX::SimpleMath::Matrix::Identity;
    DirectX::SimpleMath::Vector3 EyePosW = DirectX::SimpleMath::Vector3::Zero;
    float cbPerObjectPad1 = 0.0f;
    DirectX::SimpleMath::Vector2 RenderTargetSize = DirectX::SimpleMath::Vector2::Zero;
    DirectX::SimpleMath::Vector2 InvRenderTargetSize = DirectX::SimpleMath::Vector2::Zero;
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
};