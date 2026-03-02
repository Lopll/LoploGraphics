#pragma once

class Game;

class GameComponent
{
public:
    Game* game;
    
    GameComponent(Game* gamePtr = nullptr);
    
    virtual void DestroyResources();
    virtual void Draw();
    virtual void Initialize();
    virtual void Reload();
    virtual void Update();
};