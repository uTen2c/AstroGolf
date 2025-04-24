#include "Game.h"


Game::Game()
{
}

World& Game::GetWorld() const
{
    return *world_;
}
