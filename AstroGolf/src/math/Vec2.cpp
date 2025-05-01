#include "Vec2.h"

Vec2 Vec2::Copy() const
{
    const auto copied = *this;
    return copied;
}

Vec2 Vec2::WithX(const float& newX)
{
    x = newX;
    return *this;
}

Vec2 Vec2::WithX(const std::function<float(float)>& setter)
{
    x = setter(x);
    return *this;
}

Vec2 Vec2::WithY(const float& newY)
{
    y = newY;
    return *this;
}

Vec2 Vec2::WithY(const std::function<float(float)>& setter)
{
    y = setter(y);
    return *this;
}

Vec2 Vec2::Add(const Vec2 vec2)
{
    x += vec2.x;
    y += vec2.y;
    return *this;
}

Vec2 Vec2::Sub(const Vec2 vec2)
{
    x -= vec2.x;
    y -= vec2.y;
    return *this;
}

Vec2 Vec2::Mul(const float factor)
{
    x *= factor;
    y *= factor;
    return *this;
}

Vec2 Vec2::Neg()
{
    return Mul(-1);
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

float Vec2::Length() const
{
    return std::sqrt(x * x + y * y);
}

void Vec2::Normalize()
{
    if (const float length = Length(); length > 0)
    {
        x /= length;
        y /= length;
    }
}

Vec2 Vec2::Normalized() const
{
    Vec2 result = *this;
    if (const float length = Length(); length > 0)
    {
        result.x /= length;
        result.y /= length;
    }
    return result;
}

float Vec2::Angle(const Vec2& other) const
{
    const float dot = Dot(other);
    const float length = Length() * other.Length();
    if (length == 0.0f)
    {
        return 0;
    }
    return std::acos(dot / length);
}

Vec2 Vec2::Rotate(const float radians)
{
    const float cosRad = std::cos(radians);
    const float sinRad = std::sin(radians);
    const float newX = x * cosRad - y * sinRad;
    const float newY = x * sinRad + y * cosRad;

    x = newX;
    y = newY;

    return *this;
}

Vec2 Vec2::Rotated(const float radians) const
{
    Vec2 result;
    const float cosRad = std::cos(radians);
    const float sinRad = std::sin(radians);

    result.x = x * cosRad - y * sinRad;
    result.y = x * sinRad + y * cosRad;

    return result;
}
