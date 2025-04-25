#include "BoundingBox.h"

BoundingBox::BoundingBox(const Vec2& position, const float width, const float height)
    : position(position),
      width(width),
      height(height)
{
}

float BoundingBox::GetLeft() const
{
    return position.x;
}

float BoundingBox::GetRight() const
{
    return position.x + width;
}

float BoundingBox::GetTop() const
{
    return position.y;
}

float BoundingBox::GetBottom() const
{
    return position.y + height;
}

Vec2 BoundingBox::GetCenter() const
{
    return Vec2{position.x + width / 2, position.y + height / 2};
}

bool BoundingBox::Intersects(const Collider& other) const
{
    if (const auto* bb = dynamic_cast<const BoundingBox*>(&other))
    {
        return !(GetRight() < bb->GetLeft() ||
            GetLeft() > bb->GetRight() ||
            GetBottom() < bb->GetTop() ||
            GetTop() > bb->GetBottom());
    }
    return false;
}

bool BoundingBox::Contains(const Vec2& point) const
{
    const auto x = point.x;
    const auto y = point.y;
    return x >= GetLeft() && x <= GetRight() && y >= GetTop() && y <= GetBottom();
}
