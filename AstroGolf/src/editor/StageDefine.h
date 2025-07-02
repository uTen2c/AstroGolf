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
    float gravityMultiplier;
    bool isSatellite;
    float rotationSpeed;
    float rotationOriginOffsetX;
    float rotationOriginOffsetY;
};

class StageDefine
{
public:
    Vec2 startPos = {};
    GoalDefine goal = {};
    std::vector<PlanetDefine> planets = {};

    StageDefine() = default;

    explicit StageDefine(const json& json);

    [[nodiscard]] json ToJson() const;
};
