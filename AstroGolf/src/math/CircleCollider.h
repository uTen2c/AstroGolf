#pragma once

#include "Collider.h"
#include "Vector2d.h"

class CircleCollider : public Collider
{
    Vector2d center_; // 円の中心座標
    double radius_;   // 円の半径

public:
    CircleCollider(const Vector2d& center, double radius);

    // ゲッター
    Vector2d GetCenter() const;
    double GetRadius() const;

    // セッター（イミュータブルなので新しいインスタンスを返す）
    CircleCollider SetCenter(const Vector2d& center) const;
    CircleCollider SetRadius(double radius) const;

    bool Intersects(const Collider& other) const override;
    bool Contains(const Vector2d& point) const override;
};
