#include "BoundingBox.h"

#include "CircleCollider.h"

float BoundingBox::GetLeft() const
{
    return width * -0.5f;
}

float BoundingBox::GetRight() const
{
    return width * 0.5f;
}

float BoundingBox::GetTop() const
{
    return height * -0.5f;
}

float BoundingBox::GetBottom() const
{
    return height * 0.5f;
}

IntersectingResult BoundingBox::Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                           const Collider& otherCollider) const
{
    if (const auto* bb = dynamic_cast<const BoundingBox*>(&otherCollider))
    {
        const auto selfLeft = origin.x + GetLeft();
        const auto selfRight = origin.x + GetRight();
        const auto selfTop = origin.y + GetTop();
        const auto selfBottom = origin.y + GetBottom();
        const auto otherLeft = otherOrigin.x + bb->GetLeft();
        const auto otherRight = otherOrigin.x + bb->GetRight();
        const auto otherTop = otherOrigin.y + bb->GetTop();
        const auto otherBottom = otherOrigin.y + bb->GetBottom();

        const auto intersected = !(selfRight < otherLeft ||
            selfLeft > otherRight ||
            selfBottom < otherTop ||
            selfTop > otherBottom);
        if (intersected)
        {
            return {.intersected = true, .normal = {}};
        }
    }
    if (const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider))
    {
        return {.intersected = origin.Distance(otherOrigin) <= cc->radius, .normal = {}};
    }
    return NO_INTERSECTED;
}

bool BoundingBox::Contains(const Vec2& origin, const Vec2& point) const
{
    const auto x = point.x;
    const auto y = point.y;
    const auto left = origin.x + GetLeft();
    const auto right = origin.x + GetRight();
    const auto top = origin.y + GetTop();
    const auto bottom = origin.y + GetBottom();
    return x >= left && x <= right && y >= top && y <= bottom;
}

float BoundingBox::GetSize()
{
    return std::max(width, height);
}
