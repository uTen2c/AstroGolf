#pragma once
#include "Collider.h"
#include "DrawStack.h"
#include "RotatableBoxCollider.h"

class HoleCollider : public Collider
{
public:
    float width = 60.0f;
    float height = 80.0f;
    float edgeWidth = 3.0f;
    float rotation = 0.0f;

    HoleCollider();

    [[nodiscard]] IntersectingResult Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                                const Collider& otherCollider) const override;

    [[nodiscard]] IntersectingResult IntersectsArea(const Vec2& origin, const Vec2& otherOrigin,
                                                    const Collider& otherCollider) const;

    [[nodiscard]] bool Contains(const Vec2& origin, const Vec2& point) const override;
    [[nodiscard]] float GetSize() override;

    void Draw(DrawStack* stack) const override;
};
