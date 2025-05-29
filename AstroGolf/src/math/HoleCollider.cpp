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


    // const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider);
    // if (!cc)
    // {
    //     return NO_INTERSECTED;
    // }
    //
    // const auto outerCollider = RotatableBoxCollider(size, size, rotation);
    // const auto outerResult = outerCollider.Intersects(origin, otherOrigin, otherCollider);
    //
    // if (!outerResult.intersected)
    // {
    //     return NO_INTERSECTED;
    // }
    //
    // const auto internalOrigin = origin.Copy().Sub({0, cc->radius * 2});
    // const auto internalCollider = RotatableBoxCollider(size - cc->radius * 3, size - cc->radius * 2, rotation);
    // const auto result = internalCollider.Intersects(internalOrigin, otherOrigin, otherCollider);
    //
    // const auto negNormal = result.normal.Copy().Neg();
    // // spdlog::info("normal: {}, {}", result.normal.x, result.normal.y);
    //
    // const auto half = size / 2;
    // const auto p1 = Vec2(-half + cc->radius * 0.5f, -half).Rotate(rotation).Add(internalOrigin);
    // const auto p2 = Vec2(half - cc->radius * 0.5f, -half).Rotate(rotation).Add(internalOrigin);
    // const auto p3 = Vec2(half - cc->radius * 0.5f, half - cc->radius).Rotate(rotation).Add(internalOrigin);
    // const auto p4 = Vec2(-half + cc->radius * 0.5f, half - cc->radius).Rotate(rotation).Add(internalOrigin);
    //
    // // 円との衝突判定
    // const std::initializer_list<Edge> edges = {
    //     {.start = p1, .end = p2},
    //     {.start = p2, .end = p3},
    //     {.start = p3, .end = p4},
    //     {.start = p4, .end = p1}
    // };
    // for (const auto& [start, end] : edges)
    // {
    //     // ReSharper disable once CppTooWideScopeInitStatement
    //     const auto result = CheckEdge(start, end, otherOrigin, cc->radius);
    //     if (!result.intersected)
    //     {
    //         // continue;
    //         return result;
    //     }
    // }

    // 円の中に角が含まれていた場合
    // for (
    //     const auto corners = {p1, p2, p3, p4};
    //     const auto& corner : corners
    // )
    // {
    //     if (cc->Contains(otherOrigin, corner))
    //     {
    //         return NO_INTERSECTED;
    //     }
    // }

    // 円の中心点を含んでいた場合
    // if (Contains(origin, otherOrigin))
    // {
    //     const auto p12 = GetNearbyPoint(p1, p2, otherOrigin);
    //     const auto p23 = GetNearbyPoint(p2, p3, otherOrigin);
    //     const auto p34 = GetNearbyPoint(p3, p4, otherOrigin);
    //     const auto p41 = GetNearbyPoint(p4, p1, otherOrigin);
    //     const auto d23 = origin.Distance(p23);
    //     const auto d34 = origin.Distance(p34);
    //     const auto d41 = origin.Distance(p41);
    //     auto nearbyPoint = p12;
    //     auto distance = origin.Distance(nearbyPoint);
    //
    //     if (d23 < distance)
    //     {
    //         nearbyPoint = p23;
    //         distance = d23;
    //     }
    //
    //     if (d34 < distance)
    //     {
    //         nearbyPoint = p34;
    //         distance = d34;
    //     }
    //
    //     if (d41 < distance)
    //     {
    //         nearbyPoint = p41;
    //     }
    //     const auto normal = nearbyPoint.Copy().Sub(origin).Normalize();
    //     return {.intersected = true, .normal = normal, .point = nearbyPoint};
    // }

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
