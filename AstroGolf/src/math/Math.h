#pragma once

#include <numbers>

#pragma once

class Math
{
public:
    static constexpr float pi = std::numbers::pi_v<float>;
    static constexpr float deg_to_rad = pi / 180.0f;
};
