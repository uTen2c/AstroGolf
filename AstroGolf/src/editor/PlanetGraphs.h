#pragma once
#include <map>
#include <memory>
#include <string>

#include "../graph/Graph.h"

class PlanetGraphs
{
    inline static std::map<std::string, std::shared_ptr<Graph>> graphs_;

public:
    static void Load();

    static std::shared_ptr<Graph> GetGraph(const std::string& id);

private:
    static void Register(const std::string& id, const std::string& path, const int& size);
};
