#include "RotatableBoxCollider.h"

#include <spdlog/spdlog.h>

#include "CircleCollider.h"
#include "DrawStack.h"

namespace
{
    struct Edge
    {
        Vec2 start;
        Vec2 end;
    };

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

Vec2 RotatableBoxCollider::GetStartPos() const
{
    Vec2 start = {width * -0.5f, height * -0.5f};
    start.Rotate(rotation);
    return start;
}

Vec2 RotatableBoxCollider::GetEndPos() const
{
    Vec2 end = {width * 0.5f, height * 0.5f};
    end.Rotate(rotation);
    return end;
}

IntersectingResult RotatableBoxCollider::Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                                    const Collider& otherCollider) const
{
    const auto halfW = width / 2;
    const auto halfH = height / 2;
    const auto p1 = Vec2(-halfW, -halfH).Rotate(rotation).Add(origin);
    const auto p2 = Vec2(halfW, -halfH).Rotate(rotation).Add(origin);
    const auto p3 = Vec2(halfW, halfH).Rotate(rotation).Add(origin);
    const auto p4 = Vec2(-halfW, halfH).Rotate(rotation).Add(origin);

    // 円との衝突判定
    if (const auto* cc = dynamic_cast<const CircleCollider*>(&otherCollider))
    {
        const std::initializer_list<Edge> edges = {
            {.start = p1, .end = p2},
            {.start = p2, .end = p3},
            {.start = p3, .end = p4},
            {.start = p4, .end = p1}
        };
        for (const auto& [start, end] : edges)
        {
            // ReSharper disable once CppTooWideScopeInitStatement
            const auto result = CheckEdge(start, end, otherOrigin, cc->radius);
            if (result.intersected)
            {
                return result;
            }
        }

        // 円の中に角が含まれていた場合
        for (
            const auto corners = {p1, p2, p3, p4};
            const auto& corner : corners
        )
        {
            if (cc->Contains(otherOrigin, corner))
            {
                const auto normal = otherOrigin.Copy().Sub(corner).Normalize();
                return {.intersected = true, .normal = normal, .point = corner};
            }
        }

        // 円の中心点を含んでいた場合
        if (Contains(origin, otherOrigin))
        {
            const auto p12 = GetNearbyPoint(p1, p2, otherOrigin);
            const auto p23 = GetNearbyPoint(p2, p3, otherOrigin);
            const auto p34 = GetNearbyPoint(p3, p4, otherOrigin);
            const auto p41 = GetNearbyPoint(p4, p1, otherOrigin);
            const auto d23 = origin.Distance(p23);
            const auto d34 = origin.Distance(p34);
            const auto d41 = origin.Distance(p41);
            auto nearbyPoint = p12;
            auto distance = origin.Distance(nearbyPoint);

            if (d23 < distance)
            {
                nearbyPoint = p23;
                distance = d23;
            }

            if (d34 < distance)
            {
                nearbyPoint = p34;
                distance = d34;
            }

            if (d41 < distance)
            {
                nearbyPoint = p41;
            }
            const auto normal = nearbyPoint.Copy().Sub(origin).Normalize();
            return {.intersected = true, .normal = normal, .point = nearbyPoint};
        }
    }

    return NO_INTERSECTED;
}

bool RotatableBoxCollider::Contains(const Vec2& origin, const Vec2& point) const
{
    const auto halfW = width / 2;
    const auto halfH = height / 2;
    const auto p1 = Vec2(-halfW, -halfH).Rotate(rotation).Add(origin);
    const auto p2 = Vec2(halfW, -halfH).Rotate(rotation).Add(origin);
    const auto p3 = Vec2(halfW, halfH).Rotate(rotation).Add(origin);
    const auto p4 = Vec2(-halfW, halfH).Rotate(rotation).Add(origin);

    return p1.Copy().Sub(p2).Cross(point.Copy().Sub(p1)) < 0
        && p2.Copy().Sub(p3).Cross(point.Copy().Sub(p2)) < 0
        && p3.Copy().Sub(p4).Cross(point.Copy().Sub(p3)) < 0
        && p4.Copy().Sub(p1).Cross(point.Copy().Sub(p4)) < 0;
}

float RotatableBoxCollider::GetSize()
{
    return sqrt(width * width + height * height);
}

void RotatableBoxCollider::Draw(DrawStack* stack) const
{
    Collider::Draw(stack);
}
