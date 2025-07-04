#pragma once
#include <format>
#include <memory>

#include "Graph.h"

class Graphs
{
    Graphs() = default;

public:
    inline static std::unique_ptr<Graph> stageBackground;
    inline static std::unique_ptr<Graph> titleGraph;
    inline static std::unique_ptr<Graph> titleGroundGraph;
    inline static std::unique_ptr<Graph> titleGrassesGraph;
    inline static std::unique_ptr<Graph> titleCursorGraph;
    inline static std::unique_ptr<Graph> starsGraph;
    inline static std::unique_ptr<Graph> playerPowerIndicator;
    inline static std::unique_ptr<Graph> editorPlayerStartAnchor;
    inline static std::unique_ptr<Graph> goalHole;
    inline static std::unique_ptr<Graph> goalFlag;
    inline static std::unique_ptr<Graph> goalCompass;
    inline static std::vector<std::unique_ptr<Graph>> landingParticles;

    static void Load()
    {
        stageBackground = std::make_unique<Graph>("background.png", 2048, 2048);
        titleGraph = std::make_unique<Graph>("title.png", 1024, 1024);
        titleGroundGraph = std::make_unique<Graph>("title_ground.png", 2048, 2048);
        titleGrassesGraph = std::make_unique<Graph>("grasses.png", 2048, 2048);
        titleCursorGraph = std::make_unique<Graph>("title/cursor.png", 64, 64);
        starsGraph = std::make_unique<Graph>("stars.png", 32, 32);
        playerPowerIndicator = std::make_unique<Graph>("power_indicator.png", 12, 128);
        editorPlayerStartAnchor = std::make_unique<Graph>("editor/player_start_anchor.png", 24, 24);
        goalHole = std::make_unique<Graph>("hole.png", 128, 128);
        goalFlag = std::make_unique<Graph>("stage/goal_flag.png", 64, 64);
        goalCompass = std::make_unique<Graph>("stage/goal_compass.png", 128, 128);

        // 着地パーティクル読み込み
        static constexpr auto frames = 4;
        landingParticles.reserve(frames);
        for (int i = 0; i < frames; ++i)
        {
            landingParticles.emplace_back(std::make_unique<Graph>(std::format("landing_particle/landing_particle_{}.png", i), 48, 48));
        }
    }
};
