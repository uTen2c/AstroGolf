#include "StageWorld.h"

#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../component/misc/GoalScoreDisplayComponent.h"
#include "../game/StageManager.h"

StageWorld::StageWorld() = default;

void StageWorld::Init()
{
    World::Init();
    goal_score_display_ = std::make_shared<GoalScoreDisplayComponent>(NextComponentId());
    AddComponent(goal_score_display_);
}

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

void StageWorld::OnGoal()
{
    const auto& stageId = GetStageId();
    const auto& foundStage = std::ranges::find_if(StageManager::GetStages(), [stageId](const Stage& stage)
    {
        return stage.id == stageId;
    });
    std::vector<GoalChallengeType> clearedChallenges = {};
    if (foundStage != StageManager::GetStages().end())
    {
        const auto& player = GetPlayer();
        for (const auto& challenge : foundStage->challenges)
        {
            switch (challenge)
            {
            case GoalChallengeType::GoalHoleInOne:
                {
                    if (player->GetShotCount() == 1)
                    {
                        clearedChallenges.emplace_back(GoalChallengeType::GoalHoleInOne);
                    }
                    break;
                }
            case GoalChallengeType::GoalExcellent:
                {
                    if (player->GetShotCount() <= 3)
                    {
                        clearedChallenges.emplace_back(GoalChallengeType::GoalExcellent);
                    }
                    break;
                }
            case GoalChallengeType::Goal:
                clearedChallenges.emplace_back(GoalChallengeType::Goal);
                break;
            }
        }
    }

    spdlog::info("clear");
    for (const auto & clearedChallenge : clearedChallenges)
    {
        switch (clearedChallenge) {
        case GoalChallengeType::GoalHoleInOne:
            spdlog::info("GoalChallengeType::GoalHoleInOne");
            break;
        case GoalChallengeType::GoalExcellent:
            spdlog::info("GoalChallengeType::GoalExcellent");
            break;
        case GoalChallengeType::Goal:
            spdlog::info("GoalChallengeType::Goal");
            break;
        }
    }

    goaled_ = true;
    UpdateGoalText(); 

}

void StageWorld::UpdateGoalText() const
{
    const auto& player = GetPlayer();
    if (
        const auto& shotCount = player->GetShotCount();
        shotCount == 1
    )
    {
        goal_score_display_->SetScoreType(GoalScoreType::HoleInOne);
    }
    else if (shotCount <= 3)
    {
        goal_score_display_->SetScoreType(GoalScoreType::Excellent);
    }
    else if (shotCount <= 6)
    {
        goal_score_display_->SetScoreType(GoalScoreType::Awesome);
    }
    else
    {
        goal_score_display_->SetScoreType(GoalScoreType::Good);
    }
}

bool StageWorld::IsGoaled() const
{
    return goaled_;
}

bool StageWorld::CanPlayerShot()
{
    return World::CanPlayerShot() && !IsGoaled();
}
