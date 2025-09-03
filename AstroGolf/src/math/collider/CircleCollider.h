#pragma once

#include "Collider.h"

class CircleCollider final : public Collider
{

public:
    float radius; // 円の半径

    explicit CircleCollider(const float radius) : radius(radius) {}

    [[nodiscard]] IntersectingResult
    Intersects(const Vec2& origin, const Vec2& otherOrigin, const Collider& otherCollider) const override;
    [[nodiscard]] bool Contains(const Vec2& origin, const Vec2& point) const override;
    [[nodiscard]] float GetSize() override;
};
