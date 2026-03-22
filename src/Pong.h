#pragma once

#include "Game.h"

class Pong : public Game
{
public:
    Pong();
        
    void Update(float dt) override;
    void UpdateInput() override;
};