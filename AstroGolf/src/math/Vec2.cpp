#include "Vec2.h"

void Vec2::SetX(const std::function<float(float)>& setter)
{
    x = setter(x);
}

void Vec2::SetY(const std::function<float(float)>& setter)
{
    y = setter(y);
}

void Vec2::Add(const Vec2 vec2)
{
    x += vec2.x;
    y += vec2.y;
}

void Vec2::Multiply(const float factor)
{
    x *= factor;
    y *= factor;
}

float Vec2::Dot(const Vec2& other) const
{
    return x * other.x + y * other.y;
}

float Vec2::Distance(const Vec2& other) const
{
    const float dx = x - other.x;
    const float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}
