// double jump

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
    Vector4 materialSpecular;
    Vector3 materialAmbient;
    float materialAlpha;
    
    ModelAsset(const std::string& n, const std::string& m, const wchar_t* t, Vector4 spec, Vector3 amb, float alpha)
        : name(n), modelPath(m), texturePath(t), materialSpecular(spec), materialAmbient(amb), materialAlpha(alpha) {}
};

const std::vector<ModelAsset> AVAILABLE_MODELS = {
    ModelAsset("Ball", "./Models./Ball.obj", L"./Models./ball_basecolor.dds", Vector4(0.3f, 0.3f, 0.3f, 1.f), Vector3(0.8f, 0.6f, 0.5f), 8.f),
    ModelAsset("OldChair", "./Models/old_chair.obj", L"./Models/old_chair_diffuse.dds", Vector4(0.1f, 0.1f, 0.1f, 1.f), Vector3(0.4f, 0.2f, 0.1f), 4.f),
    ModelAsset("ChildChair", "./Models./childrens_chair.obj", L"./Models./childrens_chair_diffuse.dds", Vector4(0.3f, 0.3f, 0.3f, 1.f), Vector3(0.8f, 0.6f, 0.5f), 32.f),
    ModelAsset("Table", "./Models./coffee_table.obj", L"./Models./coffee_table_diffuse.dds", Vector4(0.773911f, 0.773911f, 0.773911f, 1.f), Vector3(0.23125f, 0.23125f, 0.23125f), 4.f)
};

float floorY = 0.f;
float ySpeed = 0.f;
float g = 30;
float jumpForce = 35;
int jumpCount = 0;

int objectCount = 10;
inline int width = 640;
inline int height = 640;

inline std::random_device katamariRd;
inline std::mt19937 katamariGen(katamariRd());
inline std::uniform_real_distribution<> scaleDist(0.05, 2);      
inline std::uniform_real_distribution<> posDist(-400.0f, 400.0f);   
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
	struct ModelAsset ballModel = AVAILABLE_MODELS[0];
	if (LoadOBJModel(ballModel.modelPath, vertices, indices, Color(1,1,1,1)))
	{
	    Entities["Ball"].transform.Translation = Vector3(400, 0, 0);
	    Entities["Ball"].transform.Scale = Vector3(1);
	    Entities["Ball"].transform.Rotation = Vector3(90, 0, 0);
		Ball = Entities["Ball"].AddComponent<RenderComponent>("Mesh", Color(1,1,1,1),nullptr,vertices, indices, ballModel.texturePath);
		Ball->constantData.MaterialSpecular = ballModel.materialSpecular;
		Ball->constantData.MaterialAmbient = ballModel.materialAmbient;
		Ball->constantData.MaterialAlpha = ballModel.materialAlpha;
		
		BallCol = Entities["Ball"].AddComponent<CollisionSphereComponent>("Collision", Vector3(), Ball->CalculateBoundingRadius());
	}
	
	for(int i = 0; i < objectCount; i++)
	{
		std::vector<Vertex> vert;
		std::vector<uint32_t> ind;
		struct ModelAsset rndModel = AVAILABLE_MODELS[modelDist(katamariGen)];
		if (LoadOBJModel(rndModel.modelPath, vert, ind, Color(1,1,1,1)))
		{
		    Entities["Object_"+std::to_string(i)].transform.Translation = Vector3(posDist(katamariGen), 0.f, posDist(katamariGen));
		    Entities["Object_"+std::to_string(i)].transform.Scale = Vector3(scaleDist(katamariGen));
			RenderComponent* rend = Entities["Object_"+std::to_string(i)].AddComponent<RenderComponent>("Mesh", Color(1,1,1,1),nullptr,vert, ind, rndModel.texturePath);
			Entities["Object_"+std::to_string(i)].AddComponent<CollisionSphereComponent>("Collision", Vector3(), rend->CalculateBoundingRadius());
			rend->constantData.MaterialSpecular = rndModel.materialSpecular;
			rend->constantData.MaterialAmbient = rndModel.materialAmbient;
			rend->constantData.MaterialAlpha = rndModel.materialAlpha;
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
	if( (movementInput.y or movementInput.x) && jumpCount == 0)
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
	
	
	// ImGui::Begin("Collision Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
	for(int i = 0; i < objectCount; i++)
	{
		CollisionSphereComponent* col = Entities["Object_"+std::to_string(i)].GetComponent<CollisionSphereComponent>("Collision");
		
		if(col)
		{
			DirectX::ContainmentType type = BallCol->bounds.Contains(col->bounds);
			const char* typeStr = (type == DirectX::CONTAINS) ? "CONTAINS" : (type == DirectX::INTERSECTS) ? "INTERSECTS" : "DISJOINT";
			// ImGui::TextColored(type == DirectX::CONTAINS ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0.5f, 0, 1), "Ball-object_%d Contains result: %s", i,typeStr);
		    
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
				    
				    Matrix ballWorld = Matrix::CreateScale(Ball->transform.Scale) * Matrix::CreateFromYawPitchRoll(Ball->transform.Rotation) * Matrix::CreateTranslation(Entities["Ball"].transform.Translation);
					Matrix worldToLocal = ballWorld.Invert();
				    
					Matrix objectWorld = Matrix::CreateScale(objRender->transform.Scale) * Matrix::CreateFromYawPitchRoll(objRender->transform.Rotation) * Matrix::CreateTranslation(objRender->transform.Translation);
					objectWorld *= worldToLocal;
					
					Vector3 scale;
					Quaternion rotation;
					Vector3 translation;
					objectWorld.Decompose(scale, rotation, translation);
					
					objRender->transform.Translation = translation;
					objRender->transform.Rotation = rotation.ToEuler();
					objRender->transform.Scale = scale;
					
					BallCol->bounds.Radius = ballR + colR;
		    		RemoveComponentFromEntity("Object_"+std::to_string(i), "Collision");
		    	}
		    	else
		    	{
		    		
		    	}
		    }
		}
	}
	// ImGui::End();
	
	// grabity
	if(Ball->transform.Translation.y > floorY)
	{
		ySpeed -= g * dt;
	}
	else if (jumpCount != 0 && Ball->transform.Translation.y != floorY)
	{
		jumpCount = 0;
		ySpeed = 0.f;
		Ball->transform.Translation.y = floorY;
	}
	if(ySpeed != 0.f)
	{
		Ball->transform.Translation.y += ySpeed * dt;
	}
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
	if(Input.IsKeyDown(Keys::Space) && jumpCount <= 2)
	{
		ySpeed = jumpForce;
		jumpCount++;
		Sleep(100);
	}
}