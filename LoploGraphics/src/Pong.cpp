#include "Pong.h"

//TODO:: 

int width = 640;
int height = 360;
int gridStep = 50;

BlockComponent* Player1 = nullptr;
    
 float startXPos = 0.7f;

float movementSpeed = 0.01f;
Vector3 movementInput = Vector3();


Pong::Pong():
    Game(L"LoploPong", width, height)
{
    int gridLen = (height * 2) - gridStep;
    for(int i = gridStep; i <= gridLen; i+=gridStep)
    {
         Components.push_back(std::make_unique<RectangleComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector3(0.0f, aspectRatio - ((float)i/height) , 0.0f), 0.0f, Vector3(0.025f, 0.2f, 1.f), Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
    }
    
    auto playerPtr = std::make_unique<BlockComponent>(this, std::array<int, 6>{0,1,2, 0,3,2}, Vector3(-startXPos, 0.0f , 0.0f), 0.0f, Vector3(0.2f, 0.6f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    Player1 = playerPtr.get();
    Components.push_back(std::move(playerPtr));

    //Components.push_back(std::make_unique<BlockComponent>(this, std::array<int, 6>{0, 1, 2, 0, 3, 2}, Vector3(-1.0f, 0.0f, 0.0f), 0.0f, Vector3(0.75f, 1.f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
    
}

void Pong::Update()
{
    // Vector3 movement{};
    // if(movementInput.y)
    // {
    //     movement.y = movementInput.y * movementSpeed;
    // }
    
    if(movementInput.y)
    {
        Matrix delta = Matrix::CreateTranslation(Vector3(0.f, movementInput.y, 0.f) * movementSpeed);
        Player1->setTranslation(Player1->transform.Translation * delta);           
        movementInput.y = 0.f;
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