#include "HoleCollider.h"

#include <spdlog/spdlog.h>

HoleCollider::HoleCollider() = default;

namespace
{
    [[maybe_unused]] Vec2 GetNearbyPoint(const Vec2& p1, const Vec2& p2, const Vec2& center)
    {
        const auto n = p2.Copy().Sub(p1).Normalize();
        const auto a = center.Copy().Sub(p1);
        const auto t = n.Dot(a);
        return p1.Copy().Add(n.Copy().Mul(t));
    }

    [[maybe_unused]] IntersectingResult CheckEdge(const Vec2& p1, const Vec2& p2, const Vec2& center,
                                                  const float& radius)
    {
        const auto nearPoint = GetNearbyPoint(p1, p2, center);

        if (nearPoint.Copy().Sub(p1).Dot(nearPoint.Copy().Sub(p2)) > 0)
        {
            return NO_INTERSECTED;
        }

        if (center.Distance(nearPoint) <= radius)
        {
            const auto normal = center.Copy().Sub(nearPoint).Normalize();
            return {.intersected = true, .normal = normal, .point = nearPoint};
        }

        return NO_INTERSECTED;
    }
}

IntersectingResult HoleCollider::Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                            const Collider& otherCollider) const
{
    const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider);
    if (!cc)
    {
        return NO_INTERSECTED;
    }

    const auto halfWidth = width * 0.5f;
    const auto halfHeight = height * 0.5f;
    const auto halfEdgeWidth = edgeWidth * 0.5f;

    const auto bottomCollider = RotatableBoxCollider(width, edgeWidth, rotation);
    const auto bottomOffset = Vec2(0, halfHeight - halfEdgeWidth).Rotate(rotation);
    const auto bottomOrigin = origin.Copy().Add(bottomOffset);
    const auto bottomResult = bottomCollider.Intersects(bottomOrigin, otherOrigin, otherCollider);

    const auto leftCollider = RotatableBoxCollider(edgeWidth, height, rotation);
    const auto leftOffset = Vec2((halfWidth - halfEdgeWidth) * -1, 0).Rotate(rotation);
    const auto leftOrigin = origin.Copy().Add(leftOffset);
    const auto leftResult = leftCollider.Intersects(leftOrigin, otherOrigin, otherCollider);

    const auto rightCollider = RotatableBoxCollider(edgeWidth, height, rotation);
    const auto rightOffset = Vec2(halfWidth - halfEdgeWidth, 0).Rotate(rotation);
    const auto rightOrigin = origin.Copy().Add(rightOffset);
    const auto rightResult = rightCollider.Intersects(rightOrigin, otherOrigin, otherCollider);

    if (bottomResult.intersected && !leftResult.intersected && !rightResult.intersected)
    {
        return bottomResult;
    }

    if (!bottomResult.intersected && leftResult.intersected)
    {
        return leftResult;
    }

    if (!bottomResult.intersected && rightResult.intersected)
    {
        return rightResult;
    }

    if (bottomResult.intersected && leftResult.intersected)
    {
        return {
            .intersected = true,
            .normal = bottomResult.normal.Copy().Add(leftResult.normal).Normalize(),
            .point = bottomResult.point,
        };
    }

    if (bottomResult.intersected && rightResult.intersected)
    {
        return {
            .intersected = true,
            .normal = bottomResult.normal.Copy().Add(rightResult.normal).Normalize(),
            .point = bottomResult.point,
        };
    }
    return NO_INTERSECTED;
}

IntersectingResult HoleCollider::IntersectsArea(const Vec2& origin, const Vec2& otherOrigin,
                                                const Collider& otherCollider) const
{
    const auto internalCollider = RotatableBoxCollider(width, height, rotation);
    return internalCollider.Intersects(origin, otherOrigin, otherCollider);
}

bool HoleCollider::Contains(const Vec2& origin, const Vec2& point) const
{
    const auto internalCollider = RotatableBoxCollider(width, height, rotation);
    return internalCollider.Contains(origin, point);
}

float HoleCollider::GetSize()
{
    return sqrt(width * width + height * height);
}

void HoleCollider::Draw(DrawStack* stack) const
{
    DrawWireBox(stack, width, height);
}
