#include "CircleCollider.h"

#include "BoundingBox.h"
#include "HoleCollider.h"
#include "RotatableBoxCollider.h"

IntersectingResult CircleCollider::Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                              const Collider& otherCollider) const
{
    // 円と円の衝突判定
    if (const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider))
    {
        const auto distance = origin.Distance(otherOrigin);
        const auto intersected = distance <= radius + cc->radius;

        if (!intersected)
        {
            return NO_INTERSECTED;
        }

        const auto normal = otherOrigin.Copy().Sub(origin);
        return {true, normal};
    }

    // 円と矩形の衝突判定
    if (const auto* box = dynamic_cast<const BoundingBox*>(&otherCollider))
    {
        const auto x1 = otherOrigin.x + box->GetLeft();
        const auto y1 = otherOrigin.y + box->GetTop();
        const auto x2 = otherOrigin.x + box->GetRight();
        const auto y2 = otherOrigin.y + box->GetBottom();

        if (origin.x > x1 && origin.x < x2 && origin.y > y1 - radius && origin.y < y2 + radius)
        {
            return {.intersected = true, .normal = {}};
        }
        if (origin.x > x1 - radius && origin.x < x2 + radius && origin.y > y1 && origin.y < y2)
        {
            return {.intersected = true, .normal = {}};
        }

        const auto r2 = radius * radius;
        if (pow(x1 - origin.x, 2) + pow(y1 - origin.y, 2) < r2)
        {
            return {.intersected = true, .normal = {}};
        }

        if (pow(x2 - origin.x, 2) + pow(y1 - origin.y, 2) < r2)
        {
            return {.intersected = true, .normal = {}};
        }

        if (pow(x2 - origin.x, 2) + pow(y2 - origin.y, 2) < r2)
        {
            return {.intersected = true, .normal = {}};
        }

        if (pow(x1 - origin.x, 2) + pow(y2 - origin.y, 2) < r2)
        {
            return {.intersected = true, .normal = {}};
        }
    }

    // 円と回転可能な短形の衝突
    if (const auto* rbc = dynamic_cast<const RotatableBoxCollider*>(&otherCollider))
    {
        return rbc->Intersects(otherOrigin, origin, *this); // NOLINT(readability-suspicious-call-argument)
    }

    // 円とホールの衝突
    if (const auto* hole = dynamic_cast<const HoleCollider*>(&otherCollider))
    {
        return hole->Intersects(otherOrigin, origin, *this); // NOLINT(readability-suspicious-call-argument)
    }

    return NO_INTERSECTED;
}

bool CircleCollider::Contains(const Vec2& origin, const Vec2& point) const
{
    const auto dx = origin.x - point.x;
    const auto dy = origin.y - point.y;
    const auto distanceSquared = dx * dx + dy * dy;
    return distanceSquared <= radius * radius;
}

float CircleCollider::GetSize()
{
    return radius;
}
