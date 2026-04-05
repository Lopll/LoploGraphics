#pragma once

#include "Game.h"

class Katamari : public Game
{
public:
    Katamari();
    void Update(float dt) override;
    void UpdateInput() override;
};