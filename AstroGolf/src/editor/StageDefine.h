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

struct PlayableAreaDefine
{
    Vec2 start;
    Vec2 end;
};

class StageDefine
{
public:
    Vec2 startPos = {};
    GoalDefine goal = {};
    std::vector<PlanetDefine> planets = {};
    std::vector<PlayableAreaDefine> playableAreas = {};

    StageDefine() = default;

    explicit StageDefine(const json& json);

    [[nodiscard]] json ToJson() const;
};
