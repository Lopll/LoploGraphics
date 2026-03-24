#include "Planets.h"

#include "RectangleComponent.h"
#include "CameraComponent.h"


int width = 640;
int height = 640;

RectangleComponent* World = nullptr;

// CameraComponent* Cam = nullptr;

Planets::Planets()
{
    // Entities["Camera"].transform.Translation = Vector3(0.f, 0.f, -5.f);
    // Cam = Entities["Camera"].AddComponent<CameraComponent>("Camera");
    // Cam->SetProjectionValues(103.f, (float)width/(float)height, -0.1, 100.f);
    
    Entities["WorldBounds"].transform.Scale = Vector3((float)width, (float)height, 100.0f);
    Entities["WorldBounds"].transform.Translation = Vector3(0.0f, 0.0f, 0.0f);
    World = Entities["WorldBounds"].AddComponent<RectangleComponent>("Bounds");
}

void Planets::Update(float dt)
{

}

void Planets::UpdateInput()
{

}