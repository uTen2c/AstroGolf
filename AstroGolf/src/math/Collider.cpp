#include "Collider.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::Intersects(const Collider& other) const
{
    return false;
}

bool Collider::Contains(const Vec2& point) const
{
    return false;
}
