#include "Vector2d.h"
#include <cmath>

#include "BoundingBox.h"

double Vector2d::GetX() const
{
    return x_;
}

double Vector2d::GetY() const
{
    return y_;
}

Vector2d Vector2d::SetX(const double x) const
{
    auto copied = *this;
    copied.x_ = x;
    return copied;
}

Vector2d Vector2d::SetX(const std::function<double(double)>& setter) const
{
    auto copied = *this;
    copied.x_ = setter(x_);
    return copied;
}

Vector2d Vector2d::SetY(const double y) const
{
    auto copied = *this;
    copied.y_ = y;
    return copied;
}

Vector2d Vector2d::SetY(const std::function<double(double)>& setter) const
{
    auto copied = *this;
    copied.y_ = setter(y_);
    return copied;
}

Vector2d Vector2d::Add(const Vector2d vector2d) const
{
    auto copied = *this;
    copied.x_ += vector2d.GetX();
    copied.y_ += vector2d.GetY();
    return copied;
}

Vector2d Vector2d::AddX(const double dx) const
{
    auto copied = *this;
    copied.x_ = x_ + dx;
    return copied;
}

Vector2d Vector2d::AddY(const double dy) const
{
    auto copied = *this;
    copied.y_ = y_ + dy;
    return copied;
}

Vector2d Vector2d::Multiply(double factor) const
{
    return Vector2d(x_ * factor, y_ * factor);
}

Vector2d Vector2d::MultiplyX(double factor) const
{
    return Vector2d(x_ * factor, y_);
}

Vector2d Vector2d::MultiplyY(double factor) const
{
    return Vector2d(x_, y_ * factor);
}

double Vector2d::Dot(const Vector2d& other) const
{
    return x_ * other.x_ + y_ * other.y_;
}

double Vector2d::Distance(const Vector2d& other) const
{
    const double dx = x_ - other.x_;
    const double dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}
