#include "CircleCollider.h"
#include "BoundingBox.h"
#include <cmath>

CircleCollider::CircleCollider(const Vector2d& center, const double radius)
    : center_(center),
      radius_(radius)
{
}

Vector2d CircleCollider::GetCenter() const
{
    return center_;
}

double CircleCollider::GetRadius() const
{
    return radius_;
}

CircleCollider CircleCollider::SetCenter(const Vector2d& center) const
{
    return {center, radius_};
}

CircleCollider CircleCollider::SetRadius(const double radius) const
{
    return {center_, radius};
}

bool CircleCollider::Intersects(const Collider& other) const
{
    // 円と円の衝突判定
    if (const auto* circle = dynamic_cast<const CircleCollider*>(&other))
    {
        const double dx = center_.GetX() - circle->center_.GetX();
        const double dy = center_.GetY() - circle->center_.GetY();
        const double distance = std::sqrt(dx * dx + dy * dy);
        
        return distance <= (radius_ + circle->radius_);
    }
    
    // 円と矩形の衝突判定
    if (const auto* box = dynamic_cast<const BoundingBox*>(&other))
    {
        // 矩形の中心と円の中心との距離を計算
        const Vector2d boxCenter = box->GetCenter();
        const double dx = std::abs(center_.GetX() - boxCenter.GetX());
        const double dy = std::abs(center_.GetY() - boxCenter.GetY());
        
        // 矩形のハーフサイズ
        const double halfWidth = box->GetWidth() / 2.0;
        const double halfHeight = box->GetHeight() / 2.0;
        
        // 円の中心から矩形までの最短距離を計算
        if (dx > halfWidth + radius_) return false;
        if (dy > halfHeight + radius_) return false;
        
        if (dx <= halfWidth) return true;
        if (dy <= halfHeight) return true;
        
        // 矩形の角と円の中心との距離を計算
        const double cornerDistanceSq = 
            std::pow(dx - halfWidth, 2) + 
            std::pow(dy - halfHeight, 2);
        
        return cornerDistanceSq <= (radius_ * radius_);
    }
    
    return false;
}

bool CircleCollider::Contains(const Vector2d& point) const
{
    const double dx = center_.GetX() - point.GetX();
    const double dy = center_.GetY() - point.GetY();
    const double distanceSquared = dx * dx + dy * dy;
    
    return distanceSquared <= (radius_ * radius_);
}
