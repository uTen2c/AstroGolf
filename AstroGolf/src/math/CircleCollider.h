#pragma once

#include "Collider.h"
#include "Vec2.h"

class CircleCollider final : public Collider
{
    Vec2 center_; // 円の中心座標
    float radius_; // 円の半径

public:
    CircleCollider(const Vec2& center, float radius);

    // ゲッター
    [[nodiscard]] Vec2 GetCenter() const;
    [[nodiscard]] float GetRadius() const;

    // セッター（イミュータブルなので新しいインスタンスを返す）
    [[nodiscard]] CircleCollider SetCenter(const Vec2& center) const;
    [[nodiscard]] CircleCollider SetRadius(float radius) const;

    [[nodiscard]] bool Intersects(const Collider& other) const override;
    [[nodiscard]] bool Contains(const Vec2& point) const override;
};
