#include "BoundingBox.h"

BoundingBox::BoundingBox(const Vector2d& position, const double width, const double height)
    : position_(position),
      width_(width),
      height_(height)
{
}

Vector2d BoundingBox::GetPosition() const
{
    return position_;
}

double BoundingBox::GetWidth() const
{
    return width_;
}

double BoundingBox::GetHeight() const
{
    return height_;
}

BoundingBox BoundingBox::SetPosition(const Vector2d& position) const
{
    return {position, width_, height_};
}

BoundingBox BoundingBox::SetWidth(const double width) const
{
    return {position_, width, height_};
}

BoundingBox BoundingBox::SetHeight(const double height) const
{
    return {position_, width_, height};
}

double BoundingBox::GetLeft() const
{
    return position_.GetX();
}

double BoundingBox::GetRight() const
{
    return position_.GetX() + width_;
}

double BoundingBox::GetTop() const
{
    return position_.GetY();
}

double BoundingBox::GetBottom() const
{
    return position_.GetY() + height_;
}

Vector2d BoundingBox::GetCenter() const
{
    return Vector2d{position_.GetX() + width_ / 2, position_.GetY() + height_ / 2};
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

bool BoundingBox::Contains(const Vector2d& point) const
{
    const double x = point.GetX();
    const double y = point.GetY();
    return x >= GetLeft() && x <= GetRight() && y >= GetTop() && y <= GetBottom();
}
