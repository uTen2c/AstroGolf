#include "StageManager.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "../world/DemoWorld.h"
#include "../world/TitleWorld.h"

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

        OnRegisterWorlds();

        return true;
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to load stages: {}", e.what());
        return false;
    }
}

std::unique_ptr<World> StageManager::CreateWorld(const std::string& id)
{
    const auto& function = factories_[id];
    if (!function)
    {
        spdlog::warn("Tried to create a world with a non-exists ID: {}", id);
        return nullptr;
    }
    return function();
}

void StageManager::OnRegisterWorlds()
{
    RegisterWorld(demoId, []
    {
        return std::make_unique<DemoWorld>();
    });
    RegisterWorld(titleId, []
    {
        return std::make_unique<TitleWorld>();
    });
}

void StageManager::RegisterWorld(const std::string& id, const std::function<std::unique_ptr<World>()>& worldFactory)
{
    if (factories_.contains(id))
    {
        spdlog::warn("World factory already registered: {}", id);
        return;
    }
    factories_[id] = worldFactory;
    spdlog::info("World factory registered: {}", id);
}
