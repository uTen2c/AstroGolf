#pragma once
#include <memory>

#include "Graph.h"

class Graphs
{
    Graphs() = default;

public:
    inline static std::unique_ptr<Graph> stageBackground;

    static void Load()
    {
        stageBackground = std::make_unique<Graph>("background.png", 2048, 2048);
    }
};
