#include "Math.h"

float Math::Lerp(const float& start, const float& end, const float& delta)
{
    return start + (end - start) * delta;
}

Vec2 Math::Lerp(const Vec2& start, const Vec2& end, const float& delta)
{
    return {
        Lerp(start.x, end.x, delta),
        Lerp(start.y, end.y, delta)
    };
}

float Math::EaseOutQuart(const float delta)
{
    return 1.0f - pow(1.0f - delta, 4.0f);
}

float Math::EaseOutQuad(const float x)
{
    return 1.0f - (1.0f - x) * (1.0f - x);
}
