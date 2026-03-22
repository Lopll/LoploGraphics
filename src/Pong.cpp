#include "Pong.h"
#include "BlockComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include <DirectXCollision.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>

int width = 640;
int height = 640;
float gridStep = width/16;

BlockComponent* Player1 = nullptr;
    
float startXPos = 0.7f;

float movementSpeed = 3.f;
Vector3 movementInput = Vector3();

DirectX::BoundingBox field;


Pong::Pong():
    Game(L"LoploPong", width, height)
{
    // Grid
    float gridLen = (height * 2) - gridStep;
    for(float i = gridStep; i <= gridLen; i+=gridStep)
    {
        Entities["GridElement_" + std::to_string(i)].transform.Scale = Vector3(10.f, 40.f, 1.0f);
        Entities["GridElement_" + std::to_string(i)].transform.Translation = Vector3(0.0f, (float)height - (float)i, 0.0f);
        Entities["GridElement_" + std::to_string(i)].AddComponent<RectangleComponent>("Sprite");
    }
    
    //Player 1
    Player1 = Entities["Player1"].AddComponent<BlockComponent>("Sprite");
    Entities["Player1"].transform.Scale = Vector3(0.146 * width, 0.382 * height, 1.0f);
    Entities["Player1"].transform.Translation = Vector3(-startXPos, 0.0f, 1.0f);
    Entities["Player1"].AddComponent<CollisionComponent>("Collider");
    
    field = Entities["WorldBounds"].AddComponent<CollisionComponent>("Bounds")->bounds;
    Entities["WorldBounds"].transform.Scale = Vector3(width, height, 1.0f);
    Entities["WorldBounds"].transform.Translation = Vector3(0.0f, 0.0f, 0.0f);
}

using namespace DirectX;
void Pong::Update()
{
    Game::Update();
    if( movementInput.y or movementInput.x)
    {
        Player1->transform.Translation += Vector3(movementInput.x, movementInput.y, 0.f) * movementSpeed;
        movementInput.y = 0.f;
        movementInput.x = 0.f;
    }

    ImGui::Begin("Collision Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    auto* playerCol = Entities["Player1"].GetComponent<CollisionComponent>("Collider");
    auto* worldCol = Entities["WorldBounds"].GetComponent<CollisionComponent>("Bounds");

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "World Bounds");
    if (worldCol)
    {
        ImGui::Text("World Center:  %.2f, %.2f, %.2f", worldCol->bounds.Center.x, worldCol->bounds.Center.y, worldCol->bounds.Center.z);
        ImGui::Text("World Extents: %.2f, %.2f, %.2f", worldCol->bounds.Extents.x, worldCol->bounds.Extents.y, worldCol->bounds.Extents.z);
        ImGui::Text("World Size:    %.2f x %.2f, %.2f", worldCol->bounds.Extents.x * 2, worldCol->bounds.Extents.y * 2, worldCol->bounds.Extents.z * 2);
    }
    else
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "WorldBounds collider not found!");
    }
    ImGui::Separator();

    ImGui::TextColored(ImVec4(0, 1, 1, 1), "Player 1");
    if (playerCol && Player1)
    {
        ImGui::Text("Player 1 Pos:    %.2f, %.2f, %.2f", Player1->transform.Translation.x, Player1->transform.Translation.y, Player1->transform.Translation.z);
        ImGui::Text("Player 1 Center: %.2f, %.2f, %.2f", playerCol->bounds.Center.x, playerCol->bounds.Center.y, playerCol->bounds.Center.z);
        ImGui::Text("Player 1 Extents:%.2f, %.2f, %.2f", playerCol->bounds.Extents.x, playerCol->bounds.Extents.y, playerCol->bounds.Extents.z);
        ImGui::Text("Player 1 Size:   %.2f x %.2f, %.2f", playerCol->bounds.Extents.x * 2, playerCol->bounds.Extents.y * 2, playerCol->bounds.Extents.z * 2);
    }
    else
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Player collider NOT FOUND!");
    }
    ImGui::Separator();

    if (playerCol && worldCol)
    {
        DirectX::ContainmentType type = worldCol->bounds.Contains(playerCol->bounds);

        const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" :
            (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";

        ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1),
            "Contains result: %s", typeStr);

        // Clamp
        float halfW = worldCol->bounds.Extents.x - playerCol->bounds.Extents.x / 2;
        float halfH = worldCol->bounds.Extents.y - playerCol->bounds.Extents.y / 2;

        if (halfW > 0.0f && halfH > 0.0f)
        {
            Vector3 pos = Player1->transform.Translation;
            Vector3 oldPos = pos;

            pos.x = std::clamp(pos.x, -halfW, halfW);
            pos.y = std::clamp(pos.y, -halfH, halfH);

            Player1->transform.Translation = pos;
        }
    }

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