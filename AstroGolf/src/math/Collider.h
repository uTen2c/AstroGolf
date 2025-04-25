#pragma once
#include "Vec2.h"

class Collider
{
public:
    Collider();
    virtual ~Collider();
    
    Collider(const Collider&) = default;
    Collider& operator=(const Collider&) = default;
    Collider(Collider&&) noexcept = default;
    Collider& operator=(Collider&&) noexcept = default;

    // 衝突判定
    [[nodiscard]] virtual bool Intersects(const Collider& other) const;

    // 点が内部にあるかどうかの判定
    [[nodiscard]] virtual bool Contains(const Vec2& point) const;
};
