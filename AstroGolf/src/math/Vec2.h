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

    [[nodiscard]] Vec2 Copy() const;

    Vec2 WithX(const float& newX);

    Vec2 WithX(const std::function<float(float)>& setter);

    Vec2 WithY(const float& newY);

    Vec2 WithY(const std::function<float(float)>& setter);

    Vec2 Add(Vec2 vec2);

    Vec2 Sub(Vec2 vec2);

    Vec2 Mul(float factor);
    Vec2 Mul(const Vec2& vec2);

    Vec2 Div(float factor);
    Vec2 Div(const Vec2& vec2);

    Vec2 Neg();

    [[nodiscard]] float Dot(const Vec2& other) const;

    // 2次元ベクトルの外積を計算する（z成分を返す）
    [[nodiscard]] float Cross(const Vec2& other) const;

    [[nodiscard]] float Distance(const Vec2& other) const;

    // ベクトルの長さを計算する
    [[nodiscard]] float Length() const;

    // ベクトルを正規化（単位ベクトル化）する
    Vec2 Normalize();

    // 正規化されたベクトルを返す
    [[nodiscard]] Vec2 Normalized() const;

    [[nodiscard]] float Angle(const Vec2& other) const;

    // 2つのベクトル間の符号付き角度を計算する（-π から π の範囲）
    [[nodiscard]] float SignedAngle(const Vec2& other) const;

    // ベクトルをラジアン角度で回転させる
    Vec2 Rotate(float radians);

    // ラジアン角度で回転した新しいベクトルを返す
    [[nodiscard]] Vec2 Rotated(float radians) const;

    // 四則演算の演算子オーバーロード
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float factor) const;
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator/(float factor) const;
    Vec2 operator/(const Vec2& other) const;
    Vec2 operator-() const;

    // 複合代入演算子
    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float factor);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator/=(float factor);
    Vec2& operator/=(const Vec2& other);

    // 比較演算子
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;
};

// スカラー値が左辺にくる場合の演算子オーバーロード
Vec2 operator*(float factor, const Vec2& vec);
