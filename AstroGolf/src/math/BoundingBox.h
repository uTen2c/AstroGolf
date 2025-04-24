#pragma once

#include "Collider.h"
#include "Vector2d.h"

class BoundingBox : public Collider
{
    Vector2d position_; // 左上の座標
    double width_; // 幅
    double height_; // 高さ

public:
    BoundingBox(const Vector2d& position, double width, double height);

    // ゲッター
    Vector2d GetPosition() const;
    double GetWidth() const;
    double GetHeight() const;

    // セッター（イミュータブルなので新しいインスタンスを返す）
    BoundingBox SetPosition(const Vector2d& position) const;
    BoundingBox SetWidth(double width) const;
    BoundingBox SetHeight(double height) const;

    // 境界値の取得
    double GetLeft() const;
    double GetRight() const;
    double GetTop() const;
    double GetBottom() const;

    // 中心座標の取得
    Vector2d GetCenter() const;

    bool Intersects(const Collider& other) const override;
    bool Contains(const Vector2d& point) const override;
};
