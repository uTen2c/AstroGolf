#pragma once

#include <functional>

struct Vec2
{
    float x;
    float y;

    Vec2(): x(0), y(0)
    {
    }

    Vec2(const float x, const float y)
        : x(x),
          y(y)
    {
    }

    void SetX(const std::function<float(float)>& setter);

    void SetY(const std::function<float(float)>& setter);

    void Add(Vec2 vec2);

    void Multiply(float factor);

    [[nodiscard]] float Dot(const Vec2& other) const;

    [[nodiscard]] float Distance(const Vec2& other) const;
};
