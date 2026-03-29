#include "Planets.h"

#include "RectangleComponent.h"
#include "CameraComponent.h"
#include "RectangleComponent.h"
#include "RenderComponent.h"
#include "GeometryGenerator.h"


int width = 640;
int height = 640;

Planets::Planets():
    Game(L"LoploPlanets", width, height)
{
    
	// Variant with Geometry Generator
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData mesh = geoGen.CreateBox(1, 1, 1, 2);
	std::vector<Vertex> vert;
	vert.resize(mesh.Vertices.size());
	for (int i = 0; i < mesh.Vertices.size(); i++)
	{
		vert[i].Pos = mesh.Vertices[i].Position;
		float randCol = ((i % (vert.size() / 2)) + 255) / 255;
		vert[i].Color = { mesh.Vertices[i].TexC.x, mesh.Vertices[i].TexC.y, 0, 1 };
	}
	Entities["TestRenderVariantA"].AddComponent<RenderComponent>("Cube", Color(1,1,1,1), nullptr, vert, mesh.Indices32);
	Entities["TestRenderVariantA"].transform.Translation = Vector3(-5,0,0);
	Entities["TestRenderVariantA"].transform.Scale = Vector3(20,20,20);
	// End of first variant

	// Variant with no Geometry Generator
	std::vector<Vertex> vertices =
	{
		Vertex({ Vector3(-1.0f, -1.0f, -1.0f), Color(1.f, 1.f, 1.f) }), 
		Vertex({ Vector3(-1.0f, +1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
		Vertex({ Vector3(+1.0f, +1.0f, -1.0f), Color(1.f, 1.f, 1.f) }), 
		Vertex({ Vector3(+1.0f, -1.0f, -1.0f), Color(0.f, 0.f, 0.f) }), 
		Vertex({ Vector3(-1.0f, -1.0f, +1.0f), Color(1.f, 1.f, 1.f) }), 
		Vertex({ Vector3(-1.0f, +1.0f, +1.0f), Color(0.f, 0.f, 0.f) }), 
		Vertex({ Vector3(+1.0f, +1.0f, +1.0f), Color(1.f, 1.f, 1.f) }), 
		Vertex({ Vector3(+1.0f, -1.0f, +1.0f), Color(0.f, 0.f, 0.f) })  
	};
	std::vector<std::uint32_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	Entities["TestRenderVariantB"].AddComponent<RenderComponent>("Cube", Color(1, 1, 1, 1), Entities["TestRenderVariantA"].GetComponent<RenderComponent>("Cube"), vertices, indices);
	Entities["TestRenderVariantB"].transform.Scale = Vector3(0.25f, 0.25f, 0.25f);
	Entities["TestRenderVariantB"].transform.Translation = Vector3(1, 0, 0);
	// end of second varitant
}

void Planets::Update(float dt)
{
    Game::Update(dt);
    for(auto& component : Components)
	{
		component->Update(dt);
		component->transform.Rotation.y += 0.001f;
	}
}

void Planets::UpdateInput()
{
    Game::UpdateInput();
}