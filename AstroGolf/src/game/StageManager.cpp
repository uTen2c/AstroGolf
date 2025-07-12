#include "StageManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "../editor/StageFileManager.h"
#include "../world/DemoWorld.h"
#include "../world/PlayWorld.h"

using json = nlohmann::json;

bool StageManager::LoadStages()
{
    spdlog::info("Stages loading...");

    try
    {
        std::ifstream file("assets/data/stages.json");
        if (!file.is_open())
        {
            spdlog::error("Failed to open stages.json");
            return false;
        }

        json stagesJson;
        file >> stagesJson;

        stages_.clear();
        for (const auto& stageJson : stagesJson)
        {
            Stage stage;
            stage.id = stageJson["id"];
            stage.name = stageJson["name"];
            stage.preview = stageJson["preview"];
            stage.challenges = stageJson["challenges"].get<std::vector<GoalChallengeType>>();
            stages_.push_back(stage);
        }

        spdlog::info("Loaded {} stages", stages_.size());

        return true;
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to load stages: {}", e.what());
        return false;
    }
}

std::vector<std::string> StageManager::GetStageIds()
{
    std::vector<std::string> keys;
    keys.reserve(stages_.size());
    for (const auto& stage : stages_)
    {
        keys.push_back(stage.id);
    }
    return keys;
}

std::unique_ptr<World> StageManager::CreateWorld(const std::string& id)
{
    const auto stageDefine = StageFileManager::GetOrLoadDefine(id);
    if (!stageDefine)
    {
        spdlog::warn("Tried to create a world with a non-exists ID: {}", id);
        return nullptr;
    }
    return std::make_unique<PlayWorld>(id, *stageDefine);
}

std::string StageManager::GetChallengeMessage(const GoalChallengeType type)
{
    switch (type)
    {
    case GoalChallengeType::GoalHoleInOne:
        return "ホールインワンする";
    case GoalChallengeType::GoalExcellent:
        return "3打以内でゴールする";
    case GoalChallengeType::Goal:
        return "ゴールする";
    case GoalChallengeType::NoBound:
        return "バウンドせずにゴールする";
    }
    return "";
}
