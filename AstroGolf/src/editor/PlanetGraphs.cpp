#include "PlanetGraphs.h"

#include <memory>
#include <spdlog/spdlog.h>

void PlanetGraphs::Load()
{
    Register("orange_big", "planet/orange_big.png", 512);
    Register("purple", "planet/purple.png", 350);
}

std::shared_ptr<Graph> PlanetGraphs::GetGraph(const std::string& id)
{
    if (!graphs_.contains(id))
    {
        return nullptr;
    }
    return graphs_.at(id);
}

void PlanetGraphs::Register(const std::string& id, const std::string& path, const int& size)
{
    if (graphs_.contains(id))
    {
        spdlog::warn("Planet graph \"{}\" is already registered", id);
        return;
    }
    graphs_[id] = std::make_shared<Graph>(path, size, size);
}
