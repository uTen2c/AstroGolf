#include "Collider.h"

#include <DxLib.h>

#include "DrawStack.h"

namespace
{
    void DrawLine(const Vec2& start, const Vec2& end, const unsigned int& color)
    {
        DrawLineAA(start.x, start.y, end.x, end.y, color);
    }
}

Collider::Collider() = default;

Collider::~Collider() = default;

IntersectingResult Collider::Intersects(const Vec2& origin, const Vec2& otherOrigin,
                                        const Collider& otherCollider) const
{
    return {.intersected = false, .normal = {}};
}

bool Collider::Contains(const Vec2& origin, const Vec2& point) const
{
    return false;
}

float Collider::GetSize()
{
    return 0;
}

void Collider::DrawWireBox(const DrawStack* stack, const float& width, const float& height)
{
    const auto vec = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto rot = stack->GetScreenRotate();
    const auto [halfW, halfH] = Vec2(width, height).Mul(0.5).Mul(scale);

    const auto p11 = Vec2(-halfW, -halfH).Rotate(rot);
    const auto p21 = Vec2(halfW, -halfH).Rotate(rot);
    const auto p12 = Vec2(-halfW, halfH).Rotate(rot);
    const auto p22 = Vec2(halfW, halfH).Rotate(rot);
    
    DrawLine(
        {vec.x + p11.x, vec.y + p11.y},
        {vec.x + p21.x, vec.y + p21.y},
        GetColor(0, 255, 0)
    );
    DrawLine(
        {vec.x + p11.x, vec.y + p11.y},
        {vec.x + p12.x, vec.y + p12.y},
        GetColor(0, 255, 0)
    );
    DrawLine(
        {vec.x + p22.x, vec.y + p22.y},
        {vec.x + p21.x, vec.y + p21.y},
        GetColor(0, 255, 0)
    );
    DrawLine(
        {vec.x + p22.x, vec.y + p22.y},
        {vec.x + p12.x, vec.y + p12.y},
        GetColor(0, 255, 0)
    );
}
