#include "RotatableBoxCollider.h"

#include <DxLib.h>
#include <imgui.h>

#include "CircleCollider.h"
#include "DrawStack.h"
#include "../Game.h"

namespace
{
    [[maybe_unused]] Vec2 GetNearbyPoint(const Vec2& p1, const Vec2& p2, const Vec2& center)
    {
        const auto n = p2.Copy().Sub(p1).Normalize();
        const auto a = center.Copy().Sub(p1);
        const auto t = n.Dot(a);
        return p1.Copy().Add(n.Copy().Mul(t));
    }

    [[maybe_unused]] bool CheckEdge(const Vec2& p1, const Vec2& p2, const Vec2& center, const float& radius)
    {
        const auto nearPoint = GetNearbyPoint(p1, p2, center);

        if (nearPoint.Copy().Sub(p1).Dot(nearPoint.Copy().Sub(p2)) > 0)
        {
            return false;
        }

        return center.Distance(nearPoint) < radius;
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

bool RotatableBoxCollider::Intersects(const Vec2& origin, const Vec2& otherOrigin, const Collider& otherCollider) const
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
        if (Contains(origin, otherOrigin))
        {
            return true;
        }

        return cc->Contains(otherOrigin, p1)
            || cc->Contains(otherOrigin, p2)
            || cc->Contains(otherOrigin, p3)
            || cc->Contains(otherOrigin, p4)
            || CheckEdge(p1, p2, otherOrigin, cc->radius)
            || CheckEdge(p2, p3, otherOrigin, cc->radius)
            || CheckEdge(p3, p4, otherOrigin, cc->radius)
            || CheckEdge(p4, p1, otherOrigin, cc->radius);
    }

    return false;
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
    return sqrt(width * width + height + height);
}
