#include "StageWorld.h"

#include "../Game.h"
#include "../game/StageManager.h"

std::string StageWorld::GetStageId() const
{
    return "";
}

void StageWorld::Reload() const
{
    if (auto world = StageManager::CreateWorld(GetStageId()))
    {
        Game::instance->ChangeWorldWithTransition(TransitionMode::Circle, std::move(world));
    }
}
