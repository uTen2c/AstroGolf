#include "CircleCollider.h"
#include "BoundingBox.h"
#include <cmath>

CircleCollider::CircleCollider(const Vec2& center, const float radius)
    : center_(center),
      radius_(radius)
{
}

Vec2 CircleCollider::GetCenter() const
{
    return center_;
}

float CircleCollider::GetRadius() const
{
    return radius_;
}

CircleCollider CircleCollider::SetCenter(const Vec2& center) const
{
    return {center, radius_};
}

CircleCollider CircleCollider::SetRadius(const float radius) const
{
    return {center_, radius};
}

bool CircleCollider::Intersects(const Collider& other) const
{
    // 円と円の衝突判定
    if (const auto* circle = dynamic_cast<const CircleCollider*>(&other))
    {
        const auto dx = center_.x - circle->center_.x;
        const auto dy = center_.y - circle->center_.y;
        const auto distance = std::sqrt(dx * dx + dy * dy);

        return distance <= (radius_ + circle->radius_);
    }

    // 円と矩形の衝突判定
    if (const auto* box = dynamic_cast<const BoundingBox*>(&other))
    {
        // 矩形の中心と円の中心との距離を計算
        const auto box_center = box->GetCenter();
        const auto dx = std::abs(center_.x - box_center.x);
        const auto dy = std::abs(center_.y - box_center.y);

        // 矩形のハーフサイズ
        const auto half_width = box->width / 2.0;
        const auto half_height = box->height / 2.0;

        // 円の中心から矩形までの最短距離を計算
        if (dx > half_width + radius_) return false;
        if (dy > half_height + radius_) return false;

        if (dx <= half_width) return true;
        if (dy <= half_height) return true;

        // 矩形の角と円の中心との距離を計算
        const auto corner_distance_sq =
            std::pow(dx - half_width, 2) +
            std::pow(dy - half_height, 2);

        return corner_distance_sq <= (radius_ * radius_);
    }

    return false;
}

bool CircleCollider::Contains(const Vec2& point) const
{
    const auto dx = center_.x - point.x;
    const auto dy = center_.y - point.y;
    const auto distance_squared = dx * dx + dy * dy;
    return distance_squared <= (radius_ * radius_);
}
