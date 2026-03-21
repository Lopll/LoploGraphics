#include "Pong.h"
#include "BlockComponent.h"
#include "RenderComponent.h"
#include <DirectXCollision.h>
#include <iostream>
#include <cstdlib>

int width = 640;
int height = 640;
float gridStep = 100;

BlockComponent* Player1 = nullptr;
    
float startXPos = 0.7f;

float movementSpeed = 0.01f;
Vector3 movementInput = Vector3();

DirectX::BoundingBox field;


Pong::Pong():
    Game(L"LoploPong", width, height)
{
    // gridStep /= aspectRatio;
    float gridLen = (height * 2) - gridStep;
    for(float i = gridStep; i <= gridLen; i+=gridStep)
    {
         //Components.push_back(std::make_unique<RectangleComponent>(std::array<int, 6>{0,1,2, 0,3,2}, Vector3(10.f, 40.f, 1.f), 0.0f, Vector3(0.0f, 1.f - ((float)i / height), 0.0f)));
        Entities["Wall_" + std::to_string(i)].transform.Scale = Vector3(10.f, 40.f, 1.f);
        Entities["Wall_" + std::to_string(i)].transform.Translation = Vector3(0.0f, 1.f - ((float)i / height), 0.0f);
        Entities["Wall_" + std::to_string(i)].AddComponent<RectangleComponent>("Sprite", std::array<int, 6>{0, 1, 2, 0, 3, 2});
    }
    
    //Entity Player;
    Entities["Player"].transform.Scale = Vector3(40.f, 120.0f, 1.0f);
    Entities["Player"].transform.Translation = Vector3(-startXPos, 0.0f, 1.0f);
    Entities["Player"].AddComponent<BlockComponent>("Sprite", std::array<int, 6>{0, 1, 2, 0, 3, 2}, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    Player1 = Entities["Player"].GetComponent<BlockComponent>("Sprite");
    //Player.AddComponent<BlockComponent>("Sprite");
    Entities["RenderEntity"].transform.Scale = Vector3(100.f);
    Entities["RenderEntity"].transform.Translation = Vector3(-startXPos, 0.0f, 1.0f);

    Entities["RenderEntity"].AddComponentDefault<RenderComponent>("renderer");

    
    // float left = -aspectRatio;
    // float right = aspectRatio;
    // float bottom = -1.0f;
    // float top = 1.0f;

    /*std::vector<Vector3> vert;
    vert.push_back(Vector3(1.f, 1.f, 0.0f));
    vert.push_back(Vector3(1.f, 0.f, 0.0f));
    vert.push_back(Vector3(0.f, 1.f, 0.0f));
    vert.push_back(Vector3(0.f, 1.f, 0.0f));
    DirectX::BoundingBox::CreateFromPoints(field, vert.size(), vert.data(), sizeof(Vector3));*/
    field = DirectX::BoundingBox(Vector3(0.0f, 0.0f, -100.0f), Vector3((float)Display.ClientWidth, (float)Display.ClientHeight, 100.f));
     //field = DirectX::BoundingBox(Vector3(-1.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)); 
    field.Transform(field, CalcProjectionMatrix());
    /*std::vector<Vector3> screenCorners =
    {
        Vector3(left,  bottom, 0.0f),
        Vector3(right, bottom, 0.0f),
        Vector3(right, top,    0.0f),
        Vector3(left,  top,    0.0f) 
    };

    DirectX::BoundingBox::CreateFromPoints(
        field,
        screenCorners.size(),
        screenCorners.data(),
        sizeof(Vector3)
    );*/
}

using namespace DirectX;
void Pong::Update()
{
    Game::Update();
    if( movementInput.y or movementInput.x)
    {
        Matrix delta = Matrix::CreateTranslation(Vector3(movementInput.x, movementInput.y, 0.f) * movementSpeed);
        // Player1->setTranslation(Player1->transform.Translation * delta);
        Player1->setTranslation(Vector3::Transform(Player1->transform.Translation, delta));
        movementInput.y = 0.f;
        movementInput.x = 0.f;
    }

    SimpleMath::Vector3 player_pos = SimpleMath::Vector3(
	    Player1->transform.Translation.x,
	    Player1->transform.Translation.y,
	    Player1->transform.Translation.z
    );
    
    std::string temp;
    if(field.Contains(Player1->collision) == DirectX::CONTAINS)         temp = "CONTAINS\n";
    else if (field.Contains(Player1->collision) == DirectX::INTERSECTS) temp = "INTERSECTS\n";
    else                                                                temp = "DISJOINT\n";

    ImGui::Begin("Update Info");
    ImGui::Text("Player Pos: %.3f %.3f %.3f",
        Player1->transform.Translation.x,
        Player1->transform.Translation.y,
        Player1->transform.Translation.z);

    ImGui::Text("Collision Center: %.3f %.3f %.3f",
        Player1->collision.Center.x,
        Player1->collision.Center.y,
        Player1->collision.Center.z);
    ImGui::Text("Field Center: %.3f %.3f %.3f",
        field.Center.x,
        field.Center.y,
        field.Center.z);
    ImGui::Text("Collision type: %s", temp.c_str());
    ImGui::End();
}

void Pong::UpdateInput()
{
    if(movementInput.y != 1.f)
    {
        if(Instance->Input.IsKeyDown(Keys::W))
        {
            movementInput.y += 1.f;
        }
        else if (Instance->Input.IsKeyDown(Keys::S))
        {
            movementInput.y -= 1.f;
        }
        if(Instance->Input.IsKeyDown(Keys::D))
        {
            movementInput.x += 1.f;
        }
        else if (Instance->Input.IsKeyDown(Keys::A))
        {
            movementInput.x -= 1.f;
        }
    }
}