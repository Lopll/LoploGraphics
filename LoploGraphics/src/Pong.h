#pragma once
#include "Game.h"
#include "TriangleComponent.h"

class Pong : public Game
{
public:
    Pong();

    void static UpdateInput();
};