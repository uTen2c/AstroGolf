#include "StageManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

bool StageManager::LoadStages()
{
    spdlog::info("Stages loading...");

    try
    {
        std::ifstream file("assets/stages.json");
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
