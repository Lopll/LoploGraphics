#pragma once

#include "Game.h"

class Planets : public Game
{
public:
    Planets();
    void zoomToFit(Entity entity);
    void Update(float dt) override;
    void UpdateInput() override;
};