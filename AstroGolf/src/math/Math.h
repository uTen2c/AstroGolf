#pragma once

#include <numbers>

#include "CrossPointResult.h"
#include "Vec2.h"

class Math
{
public:
    static constexpr float pi = std::numbers::pi_v<float>;
    static constexpr float deg_to_rad = pi / 180.0f;

    static float Lerp(const float& start, const float& end, const float& delta);
    static int Lerp(const int& start, const int& end, const float& delta);
    static Vec2 Lerp(const Vec2& start, const Vec2& end, const float& delta);

    static float EaseOutQuart(const float delta);

    template <class T>
    static bool InRange(const T& value, const T& min, const T& max)
    {
        return min <= value && value <= max;
    }

    static float EaseOutQuad(float x);

    static CrossPointResult GetCrossPoint(const Vec2& start0, const Vec2& end0, const Vec2& start1, const Vec2& end1);

    static Vec2 GetCenter(const Vec2& a, const Vec2& b);
};
