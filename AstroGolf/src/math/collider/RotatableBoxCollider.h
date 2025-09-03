#pragma once
#include "CircleCollider.h"
#include "Collider.h"

class RotatableBoxCollider final : public Collider
{
public:
    float width;
    float height;
    float rotation;

    RotatableBoxCollider(const float width, const float height, const float rotation)
        : width(width),
          height(height),
          rotation(rotation)
    {
    }

    [[nodiscard]] Vec2 GetStartPos() const;
    [[nodiscard]] Vec2 GetEndPos() const;

    [[nodiscard]] IntersectingResult Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                                const Collider& otherCollider) const override;
    [[nodiscard]] bool Contains(const Vec2& origin, const Vec2& point) const override;
    [[nodiscard]] float GetSize() override;

    void Draw(DrawStack* stack) const override;
};
