#pragma once
#include "Vec2.h"

class DrawStack;

struct IntersectingResult
{
    bool intersected;
    Vec2 normal;
    Vec2 point = {};
};

inline static IntersectingResult NO_INTERSECTED = {.intersected = false, .normal = {}, .point = {}};

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
    [[nodiscard]] virtual IntersectingResult Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                                        const Collider& otherCollider) const;

    // 点が内部にあるかどうかの判定
    [[nodiscard]] virtual bool Contains(const Vec2& origin, const Vec2& point) const;

    // 大まかな当たり判定のサイズ
    [[nodiscard]] virtual float GetSize();

    virtual void Draw(DrawStack* stack) const
    {
    }

protected:
    static void DrawWireBox(const DrawStack* stack, const float& width, const float& height);
};
