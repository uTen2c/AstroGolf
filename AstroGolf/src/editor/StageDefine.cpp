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
            auto planetDefine = PlanetDefine{
                .pos = {j.at("x"), j.at("y")},
                .radius = j.at("radius"),
                .graphId = j.at("graphId"),
                .gravityMultiplier = j.value("gravityMultiplier", 1.0f),
                .isSatellite = j.value("isSatellite", false),
                .rotationSpeed = j.value("rotationSpeed", 0.0f),
                .rotationOriginOffsetX = j.value("rotationOriginOffsetX", 0.0f),
                .rotationOriginOffsetY = j.value("rotationOriginOffsetY", 0.0f),
            };
            planets.emplace_back(planetDefine);
        }
    }
    catch (const std::exception& e)
    {
        //
    }
    try
    {
        for (auto j : json.at("playableAreas"))
        {
            PlayableAreaDefine playableAreaDefine = {
                .start = {j.at("start").at("x"), j.at("start").at("y")},
                .end = {j.at("end").at("x"), j.at("end").at("y")},
            };
            playableAreas.emplace_back(playableAreaDefine);
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
        auto planetJson = json{
            {"x", planet.pos.x},
            {"y", planet.pos.y},
            {"radius", planet.radius},
            {"graphId", planet.graphId},
            {"gravityMultiplier", planet.gravityMultiplier},
            {"isSatellite", planet.isSatellite},
            {"rotationSpeed", planet.rotationSpeed},
            {"rotationOriginOffsetX", planet.rotationOriginOffsetX},
            {"rotationOriginOffsetY", planet.rotationOriginOffsetY},
        };
        planetList.emplace_back(planetJson);
    }

    std::vector<json> playableAreaList;
    playableAreaList.reserve(playableAreas.size());
    for (const auto& area : playableAreas)
    {
        const json areaJson = {
            {
                "start",
                {
                    {"x", area.start.x},
                    {"y", area.start.y},
                },
            },
            {
                "end",
                {
                    {"x", area.end.x},
                    {"y", area.end.y},
                },
            }
        };
        playableAreaList.emplace_back(areaJson);
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
        {"planets", planetList},
        {"playableAreas", playableAreaList},
    };
}
