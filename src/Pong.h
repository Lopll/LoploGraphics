#pragma once

#include "Game.h"

class Pong : public Game
{
public:
    Pong();
        
    void Update() override;
    void UpdateInput() override;
};