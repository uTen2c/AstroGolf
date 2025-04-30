#include "Collider.h"

Collider::Collider() = default;

Collider::~Collider() = default;

bool Collider::Intersects(const Vec2& origin, const Vec2& otherOrigin, const Collider& otherCollider) const
{
    return false;
}

bool Collider::Contains(const Vec2& origin, const Vec2& point) const
{
    return false;
}

float Collider::GetSize()
{
    return 0;
}
