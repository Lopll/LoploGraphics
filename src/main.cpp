// MySuper3DApp.cpp
#include "Game.h"
#include "Katamari.h"

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    Katamari game;
    game.Run();
    return 0;
}