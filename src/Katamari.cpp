#include "Katamari.h"

#include "RectangleComponent.h"
#include "CameraComponent.h"
#include "RectangleComponent.h"
#include "RenderComponent.h"
#include "GeometryGenerator.h"
#include <random>
#include <vector>
#include <algorithm>

int width = 640;
int height = 640;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.01, 0.07);
std::uniform_real_distribution<> planSpeed(0.05, 0.2);


Katamari::Katamari():
    Game(L"LoploKatamari", width, height)
{
}

void Katamari::Update(float dt)
{
    Game::Update(dt);
    
    for(auto& component : Components)
	{
		component->Update(dt);
	}
}

void Katamari::UpdateInput()
{
    Game::UpdateInput();
    
}