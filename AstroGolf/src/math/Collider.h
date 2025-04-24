#pragma once
#include "Vector2d.h"

class Collider
{
public:
    virtual ~Collider() = default;

    // 衝突判定
    virtual bool Intersects(const Collider& other) const;

    // 点が内部にあるかどうかの判定
    virtual bool Contains(const Vector2d& point) const;
};
