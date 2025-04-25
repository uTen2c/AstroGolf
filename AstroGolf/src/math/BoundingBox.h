#pragma once

#include "Collider.h"
#include "Vec2.h"

class BoundingBox final : public Collider
{
public:
    Vec2 position; // 左上の座標
    float width; // 幅
    float height; // 高さ
    
    BoundingBox(const Vec2& position, float width, float height);

    // 境界値の取得
    [[nodiscard]] float GetLeft() const;
    [[nodiscard]] float GetRight() const;
    [[nodiscard]] float GetTop() const;
    [[nodiscard]] float GetBottom() const;

    // 中心座標の取得
    [[nodiscard]] Vec2 GetCenter() const;

    [[nodiscard]] bool Intersects(const Collider& other) const override;
    [[nodiscard]] bool Contains(const Vec2& point) const override;
};
