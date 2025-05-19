#include "Game.h"

#include <DxLib.h>

#include "world/DemoWorld.h"
#include "world/TitleWorld.h"


Game::Game() = default;

World& Game::GetWorld() const
{
    return *world_;
}

void Game::Update()
{
    if (CheckHitKey(KEY_INPUT_1) != 0 && world_->GetType() != WorldType::Title)
    {
        ChangeWorld<TitleWorld>();
    }
    if (CheckHitKey(KEY_INPUT_2) != 0 && world_->GetType() != WorldType::Demo)
    {
        ChangeWorld<DemoWorld>();
    }
    
    world_->Update(deltaTime);
    world_->Draw();
}
