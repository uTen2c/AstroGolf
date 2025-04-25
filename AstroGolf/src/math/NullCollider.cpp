#include "NullCollider.h"

bool NullCollider::Intersects(const Collider& other) const
{
    return false;
}

bool NullCollider::Contains(const Vec2& point) const
{
    return false;
}
