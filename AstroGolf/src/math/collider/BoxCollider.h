#pragma once

#include "Collider.h"
#include "../Vec2.h"

class BoxCollider final : public Collider
{
public:
    float width; // 幅
    float height; // 高さ

    explicit BoxCollider(const float width, const float height)
        : width(width), height(height)
    {
    }

    explicit BoxCollider(const int width, const int height)
        : width(static_cast<float>(width)), height(static_cast<float>(height))
    {
    }

    explicit BoxCollider(const Vec2& size)
        : width(size.x), height(size.y)
    {
    }

    // 境界値の取得
    [[nodiscard]] float GetLeft() const;
    [[nodiscard]] float GetRight() const;
    [[nodiscard]] float GetTop() const;
    [[nodiscard]] float GetBottom() const;

    [[nodiscard]] IntersectingResult Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                                const Collider& otherCollider) const override;
    [[nodiscard]] bool Contains(const Vec2& origin, const Vec2& point) const override;
    [[nodiscard]] float GetSize() override;
};
