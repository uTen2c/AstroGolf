#pragma once
#include <nlohmann/json.hpp>

#include "../math/Vec2.h"

using json = nlohmann::json;

class StageDefine
{
    Vec2 start_pos_;

public:
    explicit StageDefine(const json& json)
        : start_pos_(Vec2(json["start"]["x"], json["start"]["y"]))
    {
    }
};
