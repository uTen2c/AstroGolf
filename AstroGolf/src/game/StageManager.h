#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "../world/World.h"

using json = nlohmann::json;

enum class GoalChallengeType : std::uint8_t
{
    GoalHoleInOne,
    GoalExcellent,
    Goal,

    NoBound, // バウンドせずにゴール
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    GoalChallengeType,
    {
    {GoalChallengeType::GoalHoleInOne, "GoalHoleInOne"},
    {GoalChallengeType::GoalExcellent, "GoalExcellent"},
    {GoalChallengeType::Goal, "Goal"},
    {GoalChallengeType::NoBound, "NoBound"},
    })

struct Stage
{
    std::string id;
    std::string name;
    std::string preview;
    std::vector<GoalChallengeType> challenges;
};

class StageManager
{
    inline static std::vector<Stage> stages_ = {};

    StageManager() = default;

public:
    static inline const std::string demoId = "demo";
    static inline const std::string titleId = "title";

    static bool LoadStages();
    static const std::vector<Stage>& GetStages() { return stages_; }
    static std::vector<std::string> GetStageIds();
    static std::unique_ptr<World> CreateWorld(const std::string& id);

    static std::string GetChallengeMessage(GoalChallengeType type);
};
