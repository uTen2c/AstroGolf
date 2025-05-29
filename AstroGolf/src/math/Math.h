#pragma once

#include <numbers>

#include "Vec2.h"

#pragma once

class Math
{
public:
    static constexpr float pi = std::numbers::pi_v<float>;
    static constexpr float deg_to_rad = pi / 180.0f;
    
    static float Lerp(const float& start, const float& end, const float& delta);
    static Vec2 Lerp(const Vec2& start, const Vec2& end, const float& delta);
};
