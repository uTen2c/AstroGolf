#include "StageDefine.h"

StageDefine::StageDefine(const json& json)
{
    try
    {
        const auto& start = json.at("start");
        startPos = Vec2(start.at("x"), start.at("y"));
    }
    catch (const std::exception& e)
    {
        //
    }
    try
    {
        const auto& g = json.at("goal");
        goal = {
            .pos = Vec2(g.at("x"), g.at("y")),
            .rot = g.at("rot")
        };
    }
    catch (const std::exception& e)
    {
        //
    }
    try
    {
        for (auto j : json.at("planets"))
        {
            planets.emplace_back(PlanetDefine{
                .pos = {j.at("x"), j.at("y")},
                .radius = j.at("radius"),
                .graphId = j.at("graphId"),
                .gravityMultiplier = j.at("gravityMultiplier")
            });
        }
    }
    catch (const std::exception& e)
    {
        //
    }
}

json StageDefine::ToJson() const
{
    std::vector<json> planetList;
    planetList.reserve(planets.size());
    // ReSharper disable once CppUseStructuredBinding
    for (const auto& planet : planets)
    {
        planetList.emplace_back(json{
            {"x", planet.pos.x},
            {"y", planet.pos.y},
            {"radius", planet.radius},
            {"graphId", planet.graphId},
            {"gravityMultiplier", planet.gravityMultiplier},
        });
    }

    return json{
        {
            "start", {
                {"x", startPos.x},
                {"y", startPos.y},
            },
        },
        {
            "goal",
            {
                {"x", goal.pos.x},
                {"y", goal.pos.y},
                {"rot", goal.rot},
            }
        },
        {"planets", planetList}
    };
}
