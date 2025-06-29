#pragma once
#include <nlohmann/json.hpp>

#include "../math/Vec2.h"

using json = nlohmann::json;

struct GoalDefine
{
    Vec2 pos;
    float rot;
};

struct PlanetDefine
{
    Vec2 pos;
    float radius;
    std::string graphId;
};

class StageDefine
{
public:
    Vec2 startPos = {};
    GoalDefine goal = {};
    std::vector<PlanetDefine> planets = {};

    StageDefine() = default;

    explicit StageDefine(const json& json)
        : startPos(Vec2(json["start"]["x"], json["start"]["y"])),
          goal({
              .pos = Vec2(json["goal"]["x"], json["goal"]["y"]),
              .rot = json["goal"]["rot"]
          })
    {
        for (auto j : json["planets"])
        {
            planets.emplace_back(PlanetDefine{
                .pos = {j["x"], j["y"]},
                .radius = j["radius"],
                .graphId = j["graphId"]
            });
        }
    }
};
