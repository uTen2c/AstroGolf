#include "CircleCollider.h"
#include "BoundingBox.h"

bool CircleCollider::Intersects(const Vec2& origin, const Vec2& otherOrigin, const Collider& otherCollider) const
{
    // 円と円の衝突判定
    if (const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider))
    {
        const auto distance = origin.Distance(otherOrigin);
        return distance <= radius + cc->radius;
    }

    // 円と矩形の衝突判定
    if (const auto* box = dynamic_cast<const BoundingBox*>(&otherCollider))
    {
        // 矩形の中心と円の中心との距離を計算
        const auto dx = std::max(origin.x, otherOrigin.x) - std::min(origin.x, otherOrigin.x);
        const auto dy = std::max(origin.y, otherOrigin.y) - std::min(origin.y, otherOrigin.y);

        const auto x1 = otherOrigin.x + box->GetLeft();
        const auto y1 = otherOrigin.y + box->GetTop();
        const auto x2 = otherOrigin.x + box->GetRight();
        const auto y2 = otherOrigin.y + box->GetBottom();

        if (origin.x > x1 && origin.x < x2 && origin.y > y1 - radius && origin.y < y2 + radius)
        {
            return true;
        }
        if (origin.x > x1 - radius && origin.x < x2 + radius && origin.y > y1 && origin.y < y2)
        {
            return true;
        }

        const auto r2 = radius * radius;
        if (pow(x1 - origin.x, 2) + pow(y1 - origin.y, 2) < r2)
        {
            return true;
        }

        if (pow(x2 - origin.x, 2) + pow(y1 - origin.y, 2) < r2)
        {
            return true;
        }

        if (pow(x2 - origin.x, 2) + pow(y2 - origin.y, 2) < r2)
        {
            return true;
        }

        if (pow(x1 - origin.x, 2) + pow(y2 - origin.y, 2) < r2)
        {
            return true;
        }
    }

    return false;
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
