#pragma once

#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

struct ObjectConstants
{
    Matrix World = Matrix::Identity;
    Color Col = Color(1, 0, 0);
    
    Vector4 MaterialSpecular = Vector4(1);
    Vector3 MaterialAmbient = Vector3(1);
    float MaterialAlpha = 32;
    
};

struct PassConstants
{
    Matrix Proj = Matrix::Identity;
    Matrix InvProj = Matrix::Identity;
    Matrix View = Matrix::Identity;
    Matrix InvView = Matrix::Identity;
};

struct LightPass
{
    float LightIntencity = 1.f;
    Vector3 LightDirection = Vector3(0.f);
};