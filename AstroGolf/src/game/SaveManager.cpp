#include "SaveManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

void SaveManager::Load()
{
    spdlog::info("Data loading...");

    try
    {
        std::ifstream file("./save.json");
        if (!file.is_open())
        {
            spdlog::error("\"save.json\" not found");
            return;
        }

        json saveJson;
        file >> saveJson;
        file.close();

        for (const auto& data : saveJson)
        {
            const auto id = data["id"].get<std::string>();
            progresses_[id] = {
                data["cleared"],
                data["challenges"].get<std::vector<GoalChallengeType>>(),
            };
        }

        spdlog::info("Data loaded");
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to load save data: {}", e.what());
    }
}

void SaveManager::Save()
{
    spdlog::info("Saving game data...");

    if (progresses_.empty())
    {
        std::ofstream file("./save.json");
        if (!file.is_open())
        {
            spdlog::error("Failed to create save.json");
            return;
        }

        spdlog::info("Save data is empty");
        file << "[]";
        file.close();
        return;
    }

    try
    {
        json saveJson;

        for (const auto& [stageId, progress] : progresses_)
        {
            json entry = {
                {"id", stageId},
                {"cleared", progress.cleared},
                {"challenges", progress.clearedChallenges}
            };
            saveJson.emplace_back(entry);
        }

        std::ofstream file("./save.json");
        if (!file.is_open())
        {
            spdlog::error("Failed to create save.json");
            return;
        }

        file << saveJson.dump(4);
        file.close();

        spdlog::info("Game data saved successfully");
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to save game data: {}", e.what());
    }
}

std::map<std::string, StageProgressData>& SaveManager::GetProgresses()
{
    return progresses_;
}

bool SaveManager::ClearStage(const std::string& stageId)
{
    auto& data = progresses_[stageId];
    if (data.cleared)
    {
        return false;
    }
    data.cleared = true;
    return true;
}

bool SaveManager::ClearChallenge(const std::string& stageId, GoalChallengeType type)
{
    auto& data = progresses_[stageId];
    if (std::ranges::find(data.clearedChallenges, type) != data.clearedChallenges.end())
    {
        return false;
    }
    data.clearedChallenges.emplace_back(type);
    return true;
}
