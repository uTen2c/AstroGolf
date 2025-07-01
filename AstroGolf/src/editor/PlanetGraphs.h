#pragma once
#include <map>
#include <memory>
#include <string>

#include "../graph/Graph.h"

struct PlanetSpriteInfo
{
    std::shared_ptr<Graph> graph;
    float radius;
};

class PlanetGraphs
{
    inline static std::map<std::string, PlanetSpriteInfo> graphs_;

public:
    static void Load();

    static std::shared_ptr<Graph> GetGraph(const std::string& id);
    
    static PlanetSpriteInfo GetGraphInfo(const std::string& id);

    static std::vector<std::string> GetGraphIds();

private:
    static void Register(const std::string& id, const std::string& path, const int& size);
};
