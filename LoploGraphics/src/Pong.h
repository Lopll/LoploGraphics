#pragma once
#include "Game.h"
#include "TriangleComponent.h"
#include <array>
#include <iostream>
#include "RectangleComponent.h"
#include <DirectXCollision.h>
#include "BlockComponent.h"

class Pong : public Game
{
public:

    Pong();
    
    int Run() override;
    
    void static Update();
    void static UpdateInput();
};