#pragma once
#include "Collider.h"

class NullCollider final : public Collider
{
public:
    [[nodiscard]] bool Intersects(const Collider& other) const override;
    [[nodiscard]] bool Contains(const Vec2& point) const override;
};
