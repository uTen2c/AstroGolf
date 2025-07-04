#include "Vec2.h"
#include <cmath>
#include <limits>

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

Vec2 Vec2::Mul(const Vec2& vec2)
{
    x *= vec2.x;
    y *= vec2.y;
    return *this;
}

Vec2 Vec2::Div(const float factor)
{
    if (factor != 0.0f)
    {
        x /= factor;
        y /= factor;
    }
    else
    {
        x = 0.0f;
        y = 0.0f;
    }
    return *this;
}

Vec2 Vec2::Div(const Vec2& vec2)
{
    x = (vec2.x == 0.0f) ? 0.0f : x / vec2.x;
    y = (vec2.y == 0.0f) ? 0.0f : y / vec2.y;
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

float Vec2::Cross(const Vec2& other) const
{
    return x * other.y - y * other.x;
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

Vec2 Vec2::Normalize()
{
    if (const float length = Length(); length > 0)
    {
        x /= length;
        y /= length;
    }
    return *this;
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

float Vec2::SignedAngle(const Vec2& other) const
{
    // atan2を使用して符号付き角度を計算
    // Y軸が下向きの座標系なので、外積の符号を反転させる
    return std::atan2(-Cross(other), Dot(other));
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

// 四則演算の演算子オーバーロード実装
Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float factor) const
{
    return Vec2(x * factor, y * factor);
}

Vec2 Vec2::operator*(const Vec2& other) const
{
    return Vec2(x * other.x, y * other.y);
}

Vec2 Vec2::operator/(float factor) const
{
    // ゼロ除算チェック
    if (factor == 0.0f)
    {
        return Vec2(0.0f, 0.0f);
    }
    return Vec2(x / factor, y / factor);
}

Vec2 Vec2::operator/(const Vec2& other) const
{
    // ゼロ除算チェック
    const float newX = (other.x == 0.0f) ? 0.0f : x / other.x;
    const float newY = (other.y == 0.0f) ? 0.0f : y / other.y;
    return Vec2(newX, newY);
}

Vec2 Vec2::operator-() const
{
    return Vec2(-x, -y);
}

// 複合代入演算子の実装
Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float factor)
{
    x *= factor;
    y *= factor;
    return *this;
}

Vec2& Vec2::operator*=(const Vec2& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2& Vec2::operator/=(float factor)
{
    // ゼロ除算チェック
    if (factor != 0.0f)
    {
        x /= factor;
        y /= factor;
    }
    else
    {
        x = 0.0f;
        y = 0.0f;
    }
    return *this;
}

Vec2& Vec2::operator/=(const Vec2& other)
{
    // ゼロ除算チェック
    x = (other.x == 0.0f) ? 0.0f : x / other.x;
    y = (other.y == 0.0f) ? 0.0f : y / other.y;
    return *this;
}

// 比較演算子の実装
bool Vec2::operator==(const Vec2& other) const
{
    // 浮動小数点数の比較にはイプシロンを使用
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return !(*this == other);
}

// スカラー値が左辺にくる場合の演算子オーバーロード
Vec2 operator*(float factor, const Vec2& vec)
{
    return vec * factor;
}
