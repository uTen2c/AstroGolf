#include "PlanetGraphs.h"

#include <memory>
#include <ranges>
#include <spdlog/spdlog.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void PlanetGraphs::Load()
{
    spdlog::info("Planet graphs loading...");

    try
    {
        std::ifstream file("assets/data/graphs.json");
        if (!file.is_open())
        {
            spdlog::error("Failed to open graphs.json");
            return;
        }

        nlohmann::json graphsJson;
        file >> graphsJson;

        for (const auto& j : graphsJson)
        {
            Register(j["id"], j["path"], j["size"]);
        }
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to load graphs: {}", e.what());
    }
    // Register("orange_big", "planet/orange_big.png", 500);
    // Register("purple", "planet/purple.png", 350);
}

std::shared_ptr<Graph> PlanetGraphs::GetGraph(const std::string& id)
{
    return GetGraphInfo(id).graph;
}

PlanetSpriteInfo PlanetGraphs::GetGraphInfo(const std::string& id)
{
    if (!graphs_.contains(id))
    {
        return {nullptr, 0};
    }
    return graphs_.at(id);
}

std::vector<std::string> PlanetGraphs::GetGraphIds()
{
    std::vector<std::string> keys;
    keys.reserve(graphs_.size());
    for (const auto& k : graphs_ | std::views::keys)
    {
        keys.push_back(k);
    }
    return keys;
}

void PlanetGraphs::Register(const std::string& id, const std::string& path, const int& size)
{
    if (graphs_.contains(id))
    {
        spdlog::warn("Planet graph \"{}\" is already registered", id);
        return;
    }
    graphs_[id] = {
        std::make_shared<Graph>(path, size, size),
        size / 2.0f,
    };
}
