#include "RotatableBoxCollider.h"

#include <spdlog/spdlog.h>

#include "CircleCollider.h"
#include "DrawStack.h"
#include "Math.h"

namespace
{
    [[maybe_unused]] Vec2 GetNearbyPoint(const Vec2& p1, const Vec2& p2, const Vec2& center)
    {
        const auto n = p2.Copy().Sub(p1).Normalize();
        const auto a = center.Copy().Sub(p1);
        const auto t = n.Dot(a);
        return p1.Copy().Add(n.Copy().Mul(t));
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
        const auto hp1 = Vec2(-halfW, -halfH - cc->radius).Rotate(rotation).Add(origin);
        const auto hp2 = Vec2(halfW, -halfH - cc->radius).Rotate(rotation).Add(origin);
        const auto hp3 = Vec2(halfW, halfH + cc->radius).Rotate(rotation).Add(origin);
        const auto hp4 = Vec2(-halfW, halfH + cc->radius).Rotate(rotation).Add(origin);

        const auto inHeightArea = hp1.Copy().Sub(hp2).Cross(otherOrigin.Copy().Sub(hp1)) < 0
            && hp2.Copy().Sub(hp3).Cross(otherOrigin.Copy().Sub(hp2)) < 0
            && hp3.Copy().Sub(hp4).Cross(otherOrigin.Copy().Sub(hp3)) < 0
            && hp4.Copy().Sub(hp1).Cross(otherOrigin.Copy().Sub(hp4)) < 0;

        const auto wp1 = Vec2(-halfW - cc->radius, -halfH).Rotate(rotation).Add(origin);
        const auto wp2 = Vec2(halfW + cc->radius, -halfH).Rotate(rotation).Add(origin);
        const auto wp3 = Vec2(halfW + cc->radius, halfH).Rotate(rotation).Add(origin);
        const auto wp4 = Vec2(-halfW - cc->radius, halfH).Rotate(rotation).Add(origin);

        const auto inWidthArea = wp1.Copy().Sub(wp2).Cross(otherOrigin.Copy().Sub(wp1)) < 0
            && wp2.Copy().Sub(wp3).Cross(otherOrigin.Copy().Sub(wp2)) < 0
            && wp3.Copy().Sub(wp4).Cross(otherOrigin.Copy().Sub(wp3)) < 0
            && wp4.Copy().Sub(wp1).Cross(otherOrigin.Copy().Sub(wp4)) < 0;

        const auto p12 = GetNearbyPoint(p1, p2, otherOrigin);
        const auto p23 = GetNearbyPoint(p2, p3, otherOrigin);
        const auto p34 = GetNearbyPoint(p3, p4, otherOrigin);
        const auto p41 = GetNearbyPoint(p4, p1, otherOrigin);
        const auto d12 = otherOrigin.Distance(p12);
        const auto d23 = otherOrigin.Distance(p23);
        const auto d34 = otherOrigin.Distance(p34);
        const auto d41 = otherOrigin.Distance(p41);

        // 円の中心点を含んでいた場合
        if (inHeightArea && inWidthArea)
        {
            auto nearbyPoint = Vec2();
            auto distance = 99999.0f;

            if (d12 < distance)
            {
                nearbyPoint = p12;
                distance = d12;
            }

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

            const auto normal = Contains(origin, otherOrigin)
                                    ? nearbyPoint.Copy().Sub(otherOrigin).Normalize()
                                    : otherOrigin.Copy().Sub(nearbyPoint).Normalize();

            return {.intersected = true, .normal = normal, .point = nearbyPoint};
        }

        if (inHeightArea)
        {
            auto nearbyPoint = p12;

            if (d34 < d12)
            {
                nearbyPoint = p34;
            }

            const auto normal = Contains(origin, otherOrigin)
                                    ? nearbyPoint.Copy().Sub(otherOrigin).Normalize()
                                    : otherOrigin.Copy().Sub(nearbyPoint).Normalize();

            return {.intersected = true, .normal = normal, .point = nearbyPoint};
        }
        if (inWidthArea)
        {
            auto nearbyPoint = p23;

            if (d41 < d23)
            {
                nearbyPoint = p41;
            }

            const auto normal = Contains(origin, otherOrigin)
                                    ? nearbyPoint.Copy().Sub(otherOrigin).Normalize()
                                    : otherOrigin.Copy().Sub(nearbyPoint).Normalize();

            return {.intersected = true, .normal = normal, .point = nearbyPoint};
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
