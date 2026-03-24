#pragma once

#include "Game.h"

class Planets : public Game
{
public:
    Planets();

    void Update(float dt) override;
    void UpdateInput() override;
};