#include "Katamari.h"

#include "RectangleComponent.h"
#include "CameraComponent.h"
#include "RectangleComponent.h"
#include "RenderComponent.h"
#include "CollisionSphereComponent.h"
#include "GeometryGenerator.h"
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>

struct ModelAsset
{
    std::string name;
    std::string modelPath;
    const wchar_t* texturePath;
    
    ModelAsset(const std::string& n, const std::string& m, const wchar_t* t)
        : name(n), modelPath(m), texturePath(t) {}
};

const std::vector<ModelAsset> AVAILABLE_MODELS = {
    ModelAsset("Ball", "./Models./Ball.obj", L"./Models./ball_basecolor.dds"),
    ModelAsset("OldChair", "./Models/old_chair.obj", L"./Models/old_chair_diffuse.dds"),
    ModelAsset("OldChair", "./Models./childrens_chair.obj", L"./Models./childrens_chair_diffuse.dds"),
    ModelAsset("OldChair", "./Models./coffee_table.obj", L"./Models./coffee_table_diffuse.dds")
};

int objectCount = 10;
inline int width = 640;
inline int height = 640;

inline std::random_device katamariRd;
inline std::mt19937 katamariGen(katamariRd());
inline std::uniform_real_distribution<> scaleDist(0.05, 2);      
inline std::uniform_real_distribution<> posDist(-300.0f, 300.0f);   
inline std::uniform_int_distribution<> modelDist(0, 3); 

Vector2 movementInput = Vector2(0,0);
float rotationSpeed = 1.f;
RenderComponent* Ball = nullptr;
CollisionSphereComponent* BallCol = nullptr;

Katamari::Katamari():
    Game(L"LoploKatamari", width, height)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	
	if (LoadOBJModel("./Models./Ball.obj", vertices, indices, Color(1,1,1,1)))
	{
	    Entities["Ball"].transform.Translation = Vector3(350, 0, 0);
	    Entities["Ball"].transform.Scale = Vector3(1);
		Ball = Entities["Ball"].AddComponent<RenderComponent>("Mesh", Color(1,1,1,1),nullptr,vertices, indices, L"./Models./ball_basecolor.dds");
		BallCol = Entities["Ball"].AddComponent<CollisionSphereComponent>("Collision", Vector3(), Ball->CalculateBoundingRadius());
		
		// Entities["smal"].transform.Scale = Vector3(0.5);
		// Entities["smal"].transform.Translation = Vector3(0, 100, 0);
		// RenderComponent* smal = Entities["smal"].AddComponent<RenderComponent>("smal", Color(1,1,1,1),Ball,vertices, indices, L"./Models./ball_basecolor.dds");
		
	}
	
	for(int i = 0; i < objectCount; i++)
	{
		std::vector<Vertex> vert;
		std::vector<uint32_t> ind;
		struct ModelAsset rndModel = AVAILABLE_MODELS[modelDist(katamariGen)];
		if (LoadOBJModel(rndModel.modelPath, vert, ind, Color(1,1,1,1)))
		{
		    Entities["Object_"+std::to_string(i)].transform.Translation = Vector3(posDist(katamariGen), 0, posDist(katamariGen));
		    Entities["Object_"+std::to_string(i)].transform.Scale = Vector3(scaleDist(katamariGen));
			RenderComponent* rend = Entities["Object_"+std::to_string(i)].AddComponent<RenderComponent>("Mesh", Color(1,1,1,1),nullptr,vert, ind, rndModel.texturePath);
			Entities["Object_"+std::to_string(i)].AddComponent<CollisionSphereComponent>("Collision", Vector3(), rend->CalculateBoundingRadius());
		}
		else
		{
		    std::cout << "FAILED to load: " << rndModel.modelPath << std::endl; // Провал
		}
	}
}

void Katamari::zoomToFit(Entity entity)
{
	float margin = 2.f * BallCol->bounds.Radius;

	auto cam = Game::Entities["Orbital_Camera"].GetComponent<OrbitalCameraComponent>("Orbital_Camera"); 
	Matrix world = entity.GetComponent<RenderComponent>("Mesh")->constantData.World.Transpose();
	
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	world.Decompose(scale, rotation, translation);
	
	float worldRadius;
	if(entity.GetComponent<RenderComponent>("Mesh")->vertices.size() > 300)
	{
		worldRadius = max(scale.x, max(scale.y, scale.z));
	}
	else
	{
		worldRadius = 0.5f * sqrt(scale.x*scale.x + scale.y*scale.y + scale.z*scale.z);
	}
	
	cam->lookAtPos = translation;
	cam->distance = margin * aspectRatio * worldRadius / (sinf(DirectX::XMConvertToRadians(Game::fov)/2.f));
}

void Katamari::Update(float dt)
{
	if( movementInput.y or movementInput.x)
    {	
        Vector3 camRot = Game::Entities["Orbital_Camera"].transform.Rotation;
        Quaternion cameraRotation = Quaternion::CreateFromYawPitchRoll(-camRot.x, 0, 0.0f);
    	// rotation
        movementInput.Normalize();
        float speed = 2 * rotationSpeed * dt;
        Vector3 rotationAxis = Vector3(movementInput.x, 0.0f, movementInput.y);
        Quaternion deltaRotation = Quaternion::CreateFromAxisAngle(rotationAxis, speed);

        Quaternion cameraConjugate = cameraRotation;
	    cameraConjugate.Conjugate();
	    deltaRotation = cameraRotation * deltaRotation * cameraConjugate;
        
        Quaternion currentRotation;
        Vector3 scale;
        Vector3 translation;
        Ball->constantData.World.Transpose().Decompose(scale, currentRotation, translation);
        
        Quaternion newRotation = currentRotation * deltaRotation;
        
        Ball->transform.Rotation = newRotation.ToEuler();
        
        // movement
		Vector3 moveDelta = Vector3(-movementInput.y, 0.f, movementInput.x);
		moveDelta = Vector3::Transform(moveDelta, Matrix::CreateFromYawPitchRoll(camRot.x, 0, 0));
		Ball->transform.Translation += moveDelta;
        
        movementInput = Vector2(0.0f, 0.0f);
    }

	zoomToFit(Game::Entities["Ball"]);
    Game::Update(dt);
    
    for(auto& component : Components)
	{
		component->Update(dt);
	}
	
	
	ImGui::Begin("Collision Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
	for(int i = 0; i < objectCount; i++)
	{
		CollisionSphereComponent* col = Entities["Object_"+std::to_string(i)].GetComponent<CollisionSphereComponent>("Collision");
		
		if(col)
		{
			DirectX::ContainmentType type = BallCol->bounds.Contains(col->bounds);
			const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" : (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";
		    ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1), "Ball-object_%d Contains result: %s", i,typeStr);
		    
		    float maxBallScale = max(Ball->transform.Scale.x,max(Ball->transform.Scale.y,Ball->transform.Scale.z));
		    float ballR = maxBallScale * BallCol->bounds.Radius;
		    float maxObjScale = max(Entities["Object_"+std::to_string(i)].transform.Scale.x,max(Entities["Object_"+std::to_string(i)].transform.Scale.y,Entities["Object_"+std::to_string(i)].transform.Scale.z));
			float colR = col->bounds.Radius * maxObjScale;
		    
		    if(type == DirectX::INTERSECTS)
		    {
		    	if(ballR > colR)
		    	{
		    		RenderComponent* objRender = Entities["Object_"+std::to_string(i)].GetComponent<RenderComponent>("Mesh");
				    objRender->Parent = Ball;
				    
				    Vector3 worldObjPos = Entities["Object_"+std::to_string(i)].transform.Translation;
				    Vector3 worldBallPos = Entities["Ball"].transform.Translation;
				    Vector3 localPos = worldObjPos - worldBallPos;
				    
				    objRender->transform.Translation = Vector3::Transform(localPos,Matrix::CreateFromYawPitchRoll(Ball->transform.Rotation.x,Ball->transform.Rotation.y,Ball->transform.Rotation.z).Invert());
					objRender->transform.Rotation = Entities["Object_"+std::to_string(i)].transform.Rotation;
		    		
		    		BallCol->bounds.Radius = ballR + colR;
		    		RemoveComponentFromEntity("Object_"+std::to_string(i), "Collision");
		    	}
		    	else
		    	{
		    		
		    	}
		    }
		}
	}
	ImGui::End();
}

void Katamari::UpdateInput()
{
    Game::UpdateInput();
    
    if(Input.IsKeyDown(Keys::W))
    {
		movementInput.y += 1.f;
    }
    if(Input.IsKeyDown(Keys::S))
    {
		movementInput.y -= 1.f;
    }
    if(Input.IsKeyDown(Keys::D))
    {
		movementInput.x -= 1.f;
    }
    if(Input.IsKeyDown(Keys::A))
    {
		movementInput.x += 1.f;
    }
}