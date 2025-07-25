#pragma once
#include <string>

#include "World.h"
#include "../component/misc/GoalScoreDisplayComponent.h"

class StageWorld : public World
{
    std::shared_ptr<GoalScoreDisplayComponent> goal_score_display_;
    bool goaled_ = false;

public:
    StageWorld();

    void Init() override;

    [[nodiscard]] virtual std::string GetStageId() const;

    virtual void Reload() const;

    void OnGoal() override;

    void UpdateGoalText(int clearedChallengeCount) const;

    [[nodiscard]] bool IsGoaled() const;

    bool CanPlayerShot() override;
};
