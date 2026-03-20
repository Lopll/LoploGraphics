#include "Pong.h"

//TODO:: 

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
    gridStep /= aspectRatio;
    float gridLen = (height * 2) - gridStep;
    for(float i = gridStep; i <= gridLen; i+=gridStep)
    {
         Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector3(0.0f, 1.f - ((float)i/height) , 0.0f), 0.0f, Vector3(0.05f, 0.2f, 1.f), Vector4(1.0f, 1.0f, 1.0f, 1.0f))); // TODO: fix grid
    }
    
    auto playerPtr = std::make_unique<BlockComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector3(-startXPos, 0.0f , 0.0f), 0.0f, Vector3(0.2f, 0.6f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    Player1 = playerPtr.get();
    Components.push_back(std::move(playerPtr));

    //Components.push_back(std::make_unique<BlockComponent>(this, std::array<int, 6>{0, 1, 2, 0, 3, 2}, Vector3(-1.0f, 0.0f, 0.0f), 0.0f, Vector3(0.75f, 1.f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
    
    //field = DirectX::BoundingBox(Vector3(), Vector3(aspectRatio, 1.f, 1.f));
    // field
    //
    field = DirectX::BoundingBox(
        Vector3(0, 0, 0),
        Vector3(aspectRatio, 2.0f, 2.0f)   // полный размер по X = aspect*2, по Y = 2
    );
    field.Transform(field, CalcProjectionMatrix());
}

void Pong::Update()
{
    
    if( movementInput.y or movementInput.x)
    {
        Matrix delta = Matrix::CreateTranslation(Vector3(movementInput.x, movementInput.y, 0.f) * movementSpeed);
        Player1->setTranslation(Player1->transform.Translation * delta);           
        movementInput.y = 0.f;
        movementInput.x = 0.f;
    }

    if (field.Contains(Player1->collision) == DirectX::CONTAINS)
    {
        std::cout << "CONTAINS\n";
    }
    else if (field.Contains(Player1->collision) == DirectX::INTERSECTS)
    {
        std::cout << "INTERSECTS\n";
    }
    else if (field.Contains(Player1->collision) == DirectX::DISJOINT)
    {
        std::cout << "DISJOINT\n";
    }
    else
    {
        std::cout << "Else\n";
    }

    Game::Update();
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

int Pong::Run()
{
    if (!Initialize()) 
	{
		return 1;
	}
	
	while(!MessageHandler())
	{
		UpdateInternal();
		UpdateInput();
		Update();
		PrepareFrame();
		Draw();
		EndFrame();
	}
	Instance->Exit();
	return 0;
}