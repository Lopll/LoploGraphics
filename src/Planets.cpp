#include "Planets.h"

#include "RectangleComponent.h"
#include "CameraComponent.h"
#include "RectangleComponent.h"
#include "RenderComponent.h"
#include "GeometryGenerator.h"
#include <random>
#include <vector>

int width = 640;
int height = 640;

int planetsCount = 20;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.01, 0.07);
std::uniform_real_distribution<> planSpeed(0.05, 0.2);

std::vector<float> orbitAngles;
std::vector<float> orbitSpeeds;

std::vector<float> orbitMoonAngles;
std::vector<float> orbitMoonSpeeds;

void Planets::zoomToFit(Entity entity)// TODO: update camera pos in realtime with object transform
{
	float margin = 1.75;// TODO: уточнить про это в мм
	auto cam = Game::Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera"); 
	cam->lookAtPos = entity.transform.Translation;	
	cam->distance = margin * entity.transform.Scale.y / tanf((DirectX::XMConvertToRadians(Game::fov)/Game::aspectRatio) / 2.f);// ~25
	std::cout << cam->distance << std::endl;
	// Entities["Orbital_Camera"].distance = 100.f; // r / tan(fov_rad / 2)
}

Planets::Planets():
    Game(L"LoploPlanets", width, height)
{
    orbitAngles.resize(planetsCount + 1);
    orbitSpeeds.resize(planetsCount + 1);
    
    orbitMoonAngles.resize(planetsCount + 1);
    orbitMoonSpeeds.resize(planetsCount + 1);
    
    for(int i = 1; i <= planetsCount; i++)
    {
        orbitSpeeds[i] = (float)planSpeed(gen);
        orbitMoonSpeeds[i] = (float)planSpeed(gen);
    }
    orbitSpeeds[1] *= 10; 
    
	GeometryGenerator geoGen;
	// Sphere
	GeometryGenerator::MeshData meshSun = geoGen.CreateSphere(1.0f, 30, 30);
	std::vector<Vertex> vertSun;
	vertSun.resize(meshSun.Vertices.size());
	for (int i = 0; i < meshSun.Vertices.size(); i++)
	{
	    vertSun[i].Pos = meshSun.Vertices[i].Position;
	}
	Entities["Sun"].AddComponent<RenderComponent>("Sphere", Color(1.0f, 0.5f, 0.0f, 1.0f), nullptr, vertSun, meshSun.Indices32);
	Entities["Sun"].transform.Translation = Vector3(0,0,0);
	Entities["Sun"].transform.Scale = Vector3(20);
	
	// Cubes
	GeometryGenerator::MeshData mesh = geoGen.CreateBox(1, 1, 1, 2);
	std::vector<Vertex> vert;
	vert.resize(mesh.Vertices.size());
	for (int i = 0; i < mesh.Vertices.size(); i++)
	{
		vert[i].Pos = mesh.Vertices[i].Position;
	}
	
	for(int i = 1; i <= planetsCount; i++)
	{
		Entities["Planet_"+std::to_string(i)].AddComponent<RenderComponent>("Cube", Color(0, 0, 1, 1), Entities["Sun"].GetComponent<RenderComponent>("Sphere"), vert, mesh.Indices32);
		Entities["Planet_"+std::to_string(i)].transform.Scale = Vector3(0.5);
		float sign = (i % 2 == 0) ? -1.0f : 1.0f;
		Entities["Planet_" + std::to_string(i)].transform.Translation = Vector3(3 * i * sign, 0, 0);
		
		if (i % 2 != 0)
        {
            Entities["Moon_"+std::to_string(i)].AddComponent<RenderComponent>("Sphere", Color(1, 0, 0, 1), Entities["Planet_"+std::to_string(i)].GetComponent<RenderComponent>("Cube"), vertSun, meshSun.Indices32);
        }
        else
        {
            Entities["Moon_"+std::to_string(i)].AddComponent<RenderComponent>("Cube", Color(1, 0, 0, 1), Entities["Planet_"+std::to_string(i)].GetComponent<RenderComponent>("Cube"), vert, mesh.Indices32);
        }
		Entities["Moon_"+std::to_string(i)].transform.Scale = Vector3(0.5);
		Entities["Moon_"+std::to_string(i)].transform.Translation = Vector3(1.5,0,0);
	}
	
	// std::vector<Vertex> vertices =
	// {
	// 	Vertex({ Vector3(-1.0f, -1.0f, -1.0f), Color(1.f, 1.f, 1.f) }), 
	// 	Vertex({ Vector3(-1.0f, +1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, +1.0f, -1.0f), Color(1.f, 1.f, 1.f) }), 
	// 	Vertex({ Vector3(+1.0f, -1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(-1.0f, -1.0f, +1.0f), Color(1.f, 1.f, 1.f) }), 
	// 	Vertex({ Vector3(-1.0f, +1.0f, +1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, +1.0f, +1.0f), Color(1.f, 1.f, 1.f) }), 
	// 	Vertex({ Vector3(+1.0f, -1.0f, +1.0f), Color(0.f, 0.f, 0.f) })  
	// };
	// std::vector<Vertex> MoonVertices =
	// {
	// 	Vertex({ Vector3(-1.0f, -1.0f, -1.0f), Color(1.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(-1.0f, +1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, +1.0f, -1.0f), Color(1.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, -1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(-1.0f, -1.0f, +1.0f), Color(1.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(-1.0f, +1.0f, +1.0f), Color(0.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, +1.0f, +1.0f), Color(1.f, 0.f, 0.f) }), 
	// 	Vertex({ Vector3(+1.0f, -1.0f, +1.0f), Color(0.f, 0.f, 0.f) })  
	// };
	// std::vector<std::uint32_t> indices =
	// {
	// 	// front face
	// 	0, 1, 2,
	// 	0, 2, 3,

	// 	// back face
	// 	4, 6, 5,
	// 	4, 7, 6,

	// 	// left face
	// 	4, 5, 1,
	// 	4, 1, 0,

	// 	// right face
	// 	3, 2, 6,
	// 	3, 6, 7,

	// 	// top face
	// 	1, 5, 6,
	// 	1, 6, 2,

	// 	// bottom face
	// 	4, 0, 3,
	// 	4, 3, 7
	// };
	
	// for(int i = 1; i <= planetsCount; i++)
	// {
	// 	Entities["Planet_"+std::to_string(i)].AddComponent<RenderComponent>("Planet_"+std::to_string(i), Color(1, 1, 1, 1), Entities["Sun"].GetComponent<RenderComponent>("Cube"), vert, indices);
	// 	Entities["Planet_"+std::to_string(i)].transform.Scale = Vector3(1);//Vector3(0.25f, 0.25f, 0.25f);
	// 	Entities["Planet_"+std::to_string(i)].transform.Translation = Vector3(10,0,0);
		
	// 	Entities["Moon_"+std::to_string(i)].AddComponent<RenderComponent>("Moon_"+std::to_string(i), Color(1, 1, 1, 1), Entities["Planet_"+std::to_string(i)].GetComponent<RenderComponent>("Cube"), MoonVertices, indices);
	// 	Entities["Moon_"+std::to_string(i)].transform.Scale = Vector3(1);//Vector3(0.5f, 0.5f, 0.5f);
	// 	Entities["Moon_"+std::to_string(i)].transform.Translation = Vector3(-10,0,0);
	// }
}

void Planets::Update(float dt)
{
    Game::Update(dt);
    
    for(auto& component : Components)
	{
		component->Update(dt);
	}
	
    for(int i = 1; i <= planetsCount; i++)
    {
		Entities["Sun"].transform.Rotation.y += (float)dis(gen) * dt / 2;
    	
    	Entities["Planet_"+std::to_string(i)].transform.Rotation.y -= (float)dis(gen) * dt;
		Matrix rot = Matrix::CreateRotationY(orbitSpeeds[i] * dt);
		Entities["Planet_"+std::to_string(i)].transform.Translation = Vector3::Transform(Entities["Planet_"+std::to_string(i)].transform.Translation, rot);
        
        Entities["Moon_"+std::to_string(i)].transform.Rotation.y += (float)dis(gen) * dt;
		rot = Matrix::CreateRotationY(orbitMoonSpeeds[i] * dt);
		Entities["Moon_"+std::to_string(i)].transform.Translation = Vector3::Transform(Entities["Moon_"+std::to_string(i)].transform.Translation, rot);
    }
}

void Planets::UpdateInput()
{
    Game::UpdateInput();
    
    if(Input.IsKeyDown(Keys::D1))
    {
    	zoomToFit(Entities["Sun"]);
    }
    if(Input.IsKeyDown(Keys::D2))
    {
    	zoomToFit(Entities["Planet_1"]);
    }
}