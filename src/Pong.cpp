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
        Entities["Wall_" + std::to_string(i)].transform.Scale = Vector3(0.03125f, 0.125f, 1.0f);
        Entities["Wall_" + std::to_string(i)].transform.Translation = Vector3(0.0f, 1.f - ((float)i / height), 0.0f);
        Entities["Wall_" + std::to_string(i)].AddComponent<RectangleComponent>("Sprite", std::array<int, 6>{0, 1, 2, 0, 3, 2});
    }
    
    //Player 1;
    Entities["Player"].transform.Scale = Vector3(0.125f, 0.375f, 1.0f);
    Entities["Player"].transform.Translation = Vector3(-startXPos, 0.0f, 1.0f);
    Player1 = Entities["Player"].AddComponent<BlockComponent>("Sprite",
        std::array<int, 6>{0, 1, 2, 0, 3, 2},
        Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    Entities["Player"].AddComponentDefault<CollisionComponent>("collider");

    // Test Render Component
    Entities["RenderEntity"].transform.Scale = Vector3(0.3f, 0.3f, 1.0f);
    Entities["RenderEntity"].transform.Translation = Vector3(-startXPos, 0.0f, 1.0f);
    Entities["RenderEntity"].AddComponentDefault<RenderComponent>("renderer");

    // Test wall with collision
    //auto& wallEnt = Entities["Wall_100"];
    //wallEnt.transform.Scale = Vector3(10.f, 40.f, 1.f);
    //wallEnt.transform.Translation = Vector3(0.0f, 0.5f, 0.0f);
    //wallEnt.AddComponent<RectangleComponent>("Sprite", std::array<int, 6>{0, 1, 2, 0, 3, 2});
    //wallEnt.AddComponentDefault<CollisionComponent>("collider");
    
    Entities["WorldBounds"].transform.Scale = Vector3(2.0f, 2.0f, 1.0f);
    Entities["WorldBounds"].transform.Translation = Vector3(0.0f, 0.0f, 0.0f);
    Entities["WorldBounds"].AddComponentDefault<CollisionComponent>("bounds");
    field = Entities["WorldBounds"].GetComponent<CollisionComponent>("bounds")->bounds;
}

using namespace DirectX;
void Pong::Update()
{
    Game::Update();
    if( movementInput.y or movementInput.x)
    {
        //Matrix delta = Matrix::CreateTranslation(Vector3(movementInput.x, movementInput.y, 0.f) * movementSpeed);
        // Player1->setTranslation(Player1->transform.Translation * delta);
        //Player1->setTranslation(Vector3::Transform(Player1->transform.Translation, delta));
        Player1->transform.Translation += Vector3(movementInput.x, movementInput.y, 0.f) * movementSpeed;
        movementInput.y = 0.f;
        movementInput.x = 0.f;
    }

    SimpleMath::Vector3 player_pos = SimpleMath::Vector3(
	    Player1->transform.Translation.x,
	    Player1->transform.Translation.y,
	    Player1->transform.Translation.z
    );
    

    ImGui::Begin("Collision Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    auto* playerCol = Entities["Player"].GetComponent<CollisionComponent>("collider");
    auto* worldCol = Entities["WorldBounds"].GetComponent<CollisionComponent>("bounds");

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "=== WORLD BOUNDS ===");
    if (worldCol)
    {
        ImGui::Text("World Center:  %.1f, %.1f", worldCol->bounds.Center.x, worldCol->bounds.Center.y);
        ImGui::Text("World Extents: %.1f, %.1f", worldCol->bounds.Extents.x, worldCol->bounds.Extents.y);
        ImGui::Text("World Size:    %.1f x %.1f",
            worldCol->bounds.Extents.x * 2, worldCol->bounds.Extents.y * 2);
    }
    else
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "WorldBounds collider NOT FOUND!");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(0, 1, 1, 1), "=== PLAYER ===");
    if (playerCol && Player1)
    {
        ImGui::Text("Player Pos:    %.2f, %.2f",
            Player1->transform.Translation.x, Player1->transform.Translation.y);
        ImGui::Text("Player Center: %.2f, %.2f",
            playerCol->bounds.Center.x, playerCol->bounds.Center.y);
        ImGui::Text("Player Extents:%.2f, %.2f",
            playerCol->bounds.Extents.x, playerCol->bounds.Extents.y);
        ImGui::Text("Player Size:   %.2f x %.2f",
            playerCol->bounds.Extents.x * 2, playerCol->bounds.Extents.y * 2);
    }
    else
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Player collider NOT FOUND!");

    ImGui::Separator();

    if (playerCol && worldCol)
    {
        DirectX::ContainmentType type = worldCol->bounds.Contains(playerCol->bounds);

        const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" :
            (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";

        ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1),
            "Contains result: %s", typeStr);

        // === БЕЗОПАСНЫЙ CLAMP ===
        float halfW = worldCol->bounds.Extents.x - playerCol->bounds.Extents.x / 2;
        float halfH = worldCol->bounds.Extents.y - playerCol->bounds.Extents.y / 2;

        if (halfW > 0.0f && halfH > 0.0f)
        {
            Vector3 pos = Player1->transform.Translation;
            Vector3 oldPos = pos;

            pos.x = std::clamp(pos.x, -halfW, halfW);
            pos.y = std::clamp(pos.y, -halfH, halfH);

            Player1->transform.Translation = pos;

            if (pos.x != oldPos.x || pos.y != oldPos.y)
            {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "CLAMP APPLIED!");
                ImGui::Text("Old: %.2f, %.2f → New: %.2f, %.2f",
                    oldPos.x, oldPos.y, pos.x, pos.y);
            }
        }
        else
        {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "ERROR: Player larger than world! (halfW = %.2f)", halfW);
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