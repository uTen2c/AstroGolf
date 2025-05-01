#pragma once

#include <numbers>

#pragma once

class Math
{
public:
    static constexpr auto pi = std::numbers::pi;
    static constexpr auto deg_to_rad = pi / 180.0f;
};
