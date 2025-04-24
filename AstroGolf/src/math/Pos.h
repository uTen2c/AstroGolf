#pragma once
#include "Vector2d.h"

struct Pos
{
private:
    Vector2d point_; // 座標
    double rotation_; // 物体の回転(radian)

public:
    Pos(const Vector2d& point, const double rotation)
        : point_(point),
          rotation_(rotation)
    {
    }

    Vector2d GetPoint() const;

    double GetRotation() const;

    Pos SetPoint(const Vector2d& point) const;

    Pos SetRotation(const double& rotation) const;
};
