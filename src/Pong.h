#pragma once

#include "Game.h"
#include "BlockComponent.h"
#include <iostream>
#include <DirectXCollision.h>

class Pong : public Game
{
public:
    Pong();
        
    void Update() override;
    void UpdateInput() override;
};