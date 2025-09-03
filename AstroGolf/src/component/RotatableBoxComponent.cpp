#include "RotatableBoxComponent.h"

#include <DxLib.h>

#include "../math/collider/BoxCollider.h"
#include "../math/collider/RotatableBoxCollider.h"

namespace
{
    void DrawLine(const Vec2& start, const Vec2& end, const unsigned int& color)
    {
        DrawLineAA(start.x, start.y, end.x, end.y, color);
    }
}

RotatableBoxComponent::RotatableBoxComponent(const int id, float width, float height): PhysicsComponent(id),
    width(width),
    height(height)
{
    collider = std::make_unique<RotatableBoxCollider>(width, height, transform.rotation);
    is_static = true;
}


void RotatableBoxComponent::Update(const float delta)
{
    if (const auto rbc = dynamic_cast<RotatableBoxCollider*>(collider.get()))
    {
        rbc->rotation = transform.rotation;
    }

    PhysicsComponent::Update(delta);
}

void RotatableBoxComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto [halfW, halfH] = Vec2(width, height).Mul(0.5).Mul(scale);

    auto top = Vec2(0.0f, -halfH);
    auto bottom = Vec2(0.0f, halfH);

    top.Rotate(transform.rotation);
    bottom.Rotate(transform.rotation);

    const auto p11 = Vec2(-halfW, -halfH).Rotated(transform.rotation);
    const auto p21 = Vec2(halfW, -halfH).Rotated(transform.rotation);
    const auto p12 = Vec2(-halfW, halfH).Rotated(transform.rotation);
    const auto p22 = Vec2(halfW, halfH).Rotated(transform.rotation);

    DrawLine(
        {vec.x + p11.x, vec.y + p11.y},
        {vec.x + p21.x, vec.y + p21.y},
        GetColor(255, 255, 255)
    );
    DrawLine(
        {vec.x + p11.x, vec.y + p11.y},
        {vec.x + p12.x, vec.y + p12.y},
        GetColor(255, 255, 255)
    );
    DrawLine(
        {vec.x + p22.x, vec.y + p22.y},
        {vec.x + p21.x, vec.y + p21.y},
        GetColor(255, 255, 255)
    );
    DrawLine(
        {vec.x + p22.x, vec.y + p22.y},
        {vec.x + p12.x, vec.y + p12.y},
        GetColor(255, 255, 255)
    );

    DrawLine(
        {vec.x + p11.x, vec.y + p11.y},
        {vec.x + p22.x, vec.y + p22.y},
        GetColor(127, 127, 127)
    );

    DrawLine(
        {vec.x + p12.x, vec.y + p12.y},
        {vec.x + p21.x, vec.y + p21.y},
        GetColor(127, 127, 127)
    );

    // DrawCircleAA(vec.x + start.x, vec.y + start.y, 4, 8, GetColor(0, 255, 0), true);
    // DrawCircleAA(vec.x + end.x, vec.y + end.y, 4, 8, GetColor(255, 255, 0), true);

    DrawCircleAA(vec.x, vec.y, 2, 8, GetColor(255, 0, 0), true);

    stack->Pop();
}
