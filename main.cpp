#include "CGame.h"

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int main()
{
    CGame game;

    if (!game.Init(640, 480, TEXT("fortnite balls")))
        return -1;

    while (game.Update())
    {
    }

    return game.Release();
}