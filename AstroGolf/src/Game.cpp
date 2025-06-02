#include "Game.h"

#include <DxLib.h>

#include "world/DemoWorld.h"
#include "world/StageSelectWorld.h"
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
    if (CheckHitKey(KEY_INPUT_3) != 0 && world_->GetType() != WorldType::StageSelect)
    {
        ChangeWorld<StageSelectWorld>();
    }

    world_->Update(deltaTime);
    world_->Draw();
    world_->PostUpdate(deltaTime);
}
