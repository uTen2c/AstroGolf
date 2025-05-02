#pragma once

#include "Collider.h"
#include "Vec2.h"

class BoundingBox final : public Collider
{
public:
    float width; // 幅
    float height; // 高さ

    BoundingBox(const float width, const float height)
        : width(width),
          height(height)
    {
    }


    // 境界値の取得
    [[nodiscard]] float GetLeft() const;
    [[nodiscard]] float GetRight() const;
    [[nodiscard]] float GetTop() const;
    [[nodiscard]] float GetBottom() const;

    [[nodiscard]] IntersectingResult
    Intersects(const Vec2& origin, const Vec2& otherOrigin, const Collider& otherCollider) const override;
    [[nodiscard]] bool Contains(const Vec2& origin, const Vec2& point) const override;
    [[nodiscard]] float GetSize() override;
};
