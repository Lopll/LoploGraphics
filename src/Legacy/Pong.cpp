#include "Pong.h"
#include "BlockComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include <DirectXCollision.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "BallComponent.h"
#include <random>

int width = 640;
int height = 640;
float gridStep = (float)width/16;

CollisionComponent* World = nullptr;

BlockComponent* Player1 = nullptr;
BlockComponent* Player2 = nullptr;
    
float startXPos = 0.4f * width;

float movementSpeed = 500.f;
Vector3 movementInputPlayer1 = Vector3();
Vector3 movementInputPlayer2 = Vector3();

DirectX::BoundingBox field;

BallComponent* Ball = nullptr;
GameComponent* lastBallObstacle = nullptr;
bool ballBounced = false;

int Player1Score = 0;
int Player2Score = 0;

Pong::Pong():
    Game(L"LoploPong", width, height)
{
    // World Bounds    
    Entities["WorldBounds"].transform.Scale = Vector3((float)width, (float)height, 100.0f);
    Entities["WorldBounds"].transform.Translation = Vector3(0.0f, 0.0f, 0.0f);
    World = Entities["WorldBounds"].AddComponent<CollisionComponent>("Bounds");
    field = World->bounds;

    // Grid
    float gridLen = (height * 2) - gridStep;
    for(float i = gridStep; i <= gridLen; i+=gridStep)
    {
        Entities["GridElement_" + std::to_string(i)].transform.Scale = Vector3(0.013f * width, 0.034f * height, 1.0f);
        Entities["GridElement_" + std::to_string(i)].transform.Translation = Vector3(0.0f, (float)height - (float)i, 0.0f);
        Entities["GridElement_" + std::to_string(i)].AddComponent<RectangleComponent>("Sprite");
    }
    
    //Player 1
    Entities["Player1"].transform.Scale = Vector3(0.055f * width, 0.236f * height, 1.0f);
    Entities["Player1"].transform.Translation = Vector3(-startXPos, 0.0f, 0.0f);
    Entities["Player1"].AddComponent<CollisionComponent>("Collision");
    Player1 = Entities["Player1"].AddComponent<BlockComponent>("Sprite");
    
    //Player 2
    Entities["Player2"].transform.Scale = Vector3(0.055f * width, 0.236f * height, 1.0f);
    Entities["Player2"].transform.Translation = Vector3(startXPos, 0.0f, 0.0f);
    Entities["Player2"].AddComponent<CollisionComponent>("Collision");
    Player2 = Entities["Player2"].AddComponent<BlockComponent>("Sprite");
    
    // Random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0,1);
    
    // Ball
    Entities["Ball"].transform.Scale = Vector3(0.034f * width, 0.034f * width, 1.f);
    Entities["Ball"].transform.Translation = Vector3(0.f, 0.f, 0.f);
    Entities["Ball"].AddComponent<CollisionComponent>("Collision");
    Ball = Entities["Ball"].AddComponent<BallComponent>("Ball", Vector3((dis(gen) == 0) ? -1.f : -1.f ,0.f,0.f), 382.f, 1.034f);
}

using namespace DirectX;
void Pong::Update(float dt)
{   
    Game::Update(dt);
    if( movementInputPlayer1.y or movementInputPlayer1.x)
    {
        Player1->transform.Translation += Vector3(movementInputPlayer1.x, movementInputPlayer1.y, 0.f) * movementSpeed * dt;
        movementInputPlayer1.y = 0.f;
        movementInputPlayer1.x = 0.f;
    }
    
    if( movementInputPlayer2.y or movementInputPlayer2.x)
    {
        Player2->transform.Translation += Vector3(movementInputPlayer2.x, movementInputPlayer2.y, 0.f) * movementSpeed * dt;
        movementInputPlayer2.y = 0.f;
        movementInputPlayer2.x = 0.f;
    }

    // ImGui::Begin("Collision Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    auto* player1Col = Entities["Player1"].GetComponent<CollisionComponent>("Collision");
    auto* player2Col = Entities["Player2"].GetComponent<CollisionComponent>("Collision");
    auto* worldCol = Entities["WorldBounds"].GetComponent<CollisionComponent>("Bounds");
    auto* ballCol = Entities["Ball"].GetComponent<CollisionComponent>("Collision");

    // ImGui::TextColored(ImVec4(1, 1, 0, 1), "World Bounds");
    // if (worldCol)
    // {
    //     ImGui::Text("Center:  %.2f, %.2f, %.2f", worldCol->bounds.Center.x, worldCol->bounds.Center.y, worldCol->bounds.Center.z);
    //     ImGui::Text("Extents: %.2f, %.2f, %.2f", worldCol->bounds.Extents.x, worldCol->bounds.Extents.y, worldCol->bounds.Extents.z);
    //     ImGui::Text("Size:    %.2f x %.2f, %.2f", worldCol->bounds.Extents.x * 2, worldCol->bounds.Extents.y * 2, worldCol->bounds.Extents.z * 2);
    // }
    // ImGui::Separator();

    // ImGui::TextColored(ImVec4(0, 1, 1, 1), "Player 1");
    // if (playerCol && Player1)
    // {
    //     ImGui::Text("Pos:    %.2f, %.2f, %.2f", Player1->transform.Translation.x, Player1->transform.Translation.y, Player1->transform.Translation.z);
    //     ImGui::Text("Bounds Center: %.2f, %.2f, %.2f", playerCol->bounds.Center.x, playerCol->bounds.Center.y, playerCol->bounds.Center.z);
    //     ImGui::Text("Bounds Extents:%.2f, %.2f, %.2f", playerCol->bounds.Extents.x, playerCol->bounds.Extents.y, playerCol->bounds.Extents.z);
    //     ImGui::Text("Bounds Size:   %.2f x %.2f, %.2f", playerCol->bounds.Extents.x * 2, playerCol->bounds.Extents.y * 2, playerCol->bounds.Extents.z * 2);
    // }
    // ImGui::Separator();
    
    // World Collisions
    if (worldCol)
    {
        if (Player1 != NULL && player1Col)
        {
            DirectX::ContainmentType type = worldCol->bounds.Contains(player1Col->bounds);
            
            // const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" :
            //     (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";

            // ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1),
            //     "Contains result: %s", typeStr);
            
            // Clamp player
            if (type != DirectX::CONTAINS)
            {
                float halfW = worldCol->bounds.Extents.x - player1Col->bounds.Extents.x;
                float halfH = worldCol->bounds.Extents.y - player1Col->bounds.Extents.y;
            
                Vector3 pos = Player1->transform.Translation;
                Vector3 oldPos = pos;

                pos.x = std::clamp(pos.x, -halfW, halfW);
                pos.y = std::clamp(pos.y, -halfH, halfH);

                Player1->transform.Translation = pos;
            }
            // ImGui::Separator();
        }
        
        if (Player2 != NULL && player2Col)
        {
            DirectX::ContainmentType type = worldCol->bounds.Contains(player2Col->bounds);
            
            // Clamp player
            if (type != DirectX::CONTAINS)
            {
                float halfW = worldCol->bounds.Extents.x - player2Col->bounds.Extents.x;
                float halfH = worldCol->bounds.Extents.y - player2Col->bounds.Extents.y;
            
                Vector3 pos = Player2->transform.Translation;
                Vector3 oldPos = pos;

                pos.x = std::clamp(pos.x, -halfW, halfW);
                pos.y = std::clamp(pos.y, -halfH, halfH);

                Player2->transform.Translation = pos;
            }
        }
        
        // ImGui::TextColored(ImVec4(1, 1, 1, 1), "Ball direction: %.2f, %.2f, %.2f", Ball->movementDirection.x, Ball->movementDirection.y, Ball->movementDirection.z);
        
        ImGui::Begin("Score", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (Ball != NULL && ballCol)
        {
            DirectX::ContainmentType type = worldCol->bounds.Contains(ballCol->bounds);
        
            // const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" :
            //     (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";

            // ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1),
            //     "Ball-Worlds Contains result: %s", typeStr);
            
            if (type == DirectX::INTERSECTS && !ballBounced)
            {
                ballBounced = true;
                lastBallObstacle = World;
                Entities["Ball"].GetComponent<BallComponent>("Ball")->OnBoundsContact(ballCol, worldCol);
                
                // Score contact
                float scoreTrigger = width/2 - ballCol->bounds.Extents.x*2;
                if (ballCol->bounds.Center.x >= scoreTrigger)
                {
                    // Score to Player1
                    Ball->transform.Translation = Vector3();
                    Ball->movementDirection = Vector3(-1.f, 0.f, 0.f);
                    ballBounced = false;
                    Player1Score++;
                }
                else if (ballCol->bounds.Center.x <= -scoreTrigger)
                {
                    // Score to Player2
                    Ball->transform.Translation = Vector3();
                    Ball->movementDirection = Vector3(1.f, 0.f, 0.f);
                    ballBounced = false;
                    Player2Score++;
                }
            }
            else if (ballBounced && lastBallObstacle == World && type == DirectX::CONTAINS)
            {
                ballBounced = false;
            }
        }
    }
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "SCORE: %d, %d", Player1Score, Player2Score);
    
    // Ball Collisions
    if (ballCol)
    {
        if (Player1 != NULL && player1Col)
        {
            DirectX::ContainmentType type = player1Col->bounds.Contains(ballCol->bounds);
            
            // const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" :
                // (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";

            // ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1), "Ball-Player1 Contains result: %s", typeStr);
        
            if (type == DirectX::INTERSECTS && !ballBounced)
            {   
                ballBounced = true;
                lastBallObstacle = Player1;
                Entities["Ball"].GetComponent<BallComponent>("Ball")->OnPlayerContact(ballCol, player1Col);
            }
            else if (ballBounced && lastBallObstacle == Player1 && type == DirectX::DISJOINT)
            {
                ballBounced = false;
            }
        }
        
        if (Player2 != NULL && player2Col)
        {
            DirectX::ContainmentType type = player2Col->bounds.Contains(ballCol->bounds);
        
            if (type == DirectX::INTERSECTS && !ballBounced)
            {   
                ballBounced = true;
                lastBallObstacle = Player2;
                Entities["Ball"].GetComponent<BallComponent>("Ball")->OnPlayerContact(ballCol, player2Col);
            }
            else if (ballBounced && lastBallObstacle == Player2 && type == DirectX::DISJOINT)
            {
                ballBounced = false;
            }
        }
    }
    ImGui::End();
}

void Pong::UpdateInput()
{
    if(std::abs(movementInputPlayer1.y) != 1.f)
    {
        if(Instance->Input.IsKeyDown(Keys::W))
        {
            movementInputPlayer1.y += 1.f;
        }
        else if (Instance->Input.IsKeyDown(Keys::S))
        {
            movementInputPlayer1.y -= 1.f;
        }
        // if(Instance->Input.IsKeyDown(Keys::D))
        // {
        //     movementInput.x += 1.f;
        // }
        // else if (Instance->Input.IsKeyDown(Keys::A))
        // {
        //     movementInput.x -= 1.f;
        // }
    }
    
    if(std::abs(movementInputPlayer2.y) != 1.f)
    {
        if(Instance->Input.IsKeyDown(Keys::Up))
        {
            movementInputPlayer2.y += 1.f;
        }
        else if (Instance->Input.IsKeyDown(Keys::Down))
        {
            movementInputPlayer2.y -= 1.f;
        }

    }
}