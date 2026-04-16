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
    float ShowShadowMap = 0.0f;
    Vector3 padding;
};

struct PointLightData
{
    float Intencity = 1.f;
    Vector3 Position = Vector3(0.f);
    Vector3 Color = Vector3(0.f);
    float Radius = 1.f;
};

struct DirectionalLightData
{
    float Intencity = 1.f;
    Vector3 Direction = Vector3(0.f);
    // DirectionalLightData(float i, Vector3 d):Intencity(i), Direction(d){}
};

struct LightPass
{
    DirectionalLightData directionalLightData = DirectionalLightData();
    Matrix LightView = Matrix::Identity;
    PointLightData pointLightData[100];
};