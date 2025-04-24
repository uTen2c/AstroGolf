#pragma once

#include <functional>

struct Vector2d
{
private:
    double x_;
    double y_;

public:
    Vector2d(const double x, const double y)
        : x_(x),
          y_(y)
    {
    }

    double GetX() const;

    double GetY() const;

    Vector2d SetX(double x) const;

    Vector2d SetX(const std::function<double(double)>& setter) const;

    Vector2d SetY(double y) const;

    Vector2d SetY(const std::function<double(double)>& setter) const;

    Vector2d Add(Vector2d vector2d) const;

    Vector2d AddX(double dx) const;

    Vector2d AddY(double dy) const;

    Vector2d Multiply(double factor) const;
    
    Vector2d MultiplyX(double factor) const;
    
    Vector2d MultiplyY(double factor) const;
    
    double Dot(const Vector2d& other) const;

    double Distance(const Vector2d& other) const;
};
