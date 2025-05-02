#include "BoxComponent.h"

#include <DxLib.h>
#include <spdlog/spdlog.h>

#include "../math/BoundingBox.h"

namespace
{
    void DrawLine(const Vec2& start, const Vec2& end, const unsigned int& color)
    {
        DrawLineAA(start.x, start.y, end.x, end.y, color);
    }
}

BoxComponent::BoxComponent(const int id, const float width, const float height): PhysicsComponent(id), width(width),
    height(height)
{
    collider = std::make_unique<BoundingBox>(width, height);
    isStatic = true;
}

BoxComponent::~BoxComponent()
{
}

void BoxComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    const auto halfW = width * 0.5f;
    const auto halfH = height * 0.5f;

    const auto start = Vec2(-halfW, -halfH).Rotated(transform.rotation);
    const auto end = Vec2(halfW, halfH).Rotated(transform.rotation);

    auto top = Vec2(0, -halfH);
    auto bottom = Vec2(0, halfH);

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

    // DrawLineAA(vec.x + top.x, vec.y + top.y, vec.x + bottom.x, vec.y + bottom.y, GetColor(255, 0, 0), 1.0f);

    // DrawBoxAA(vec.x - halfW, vec.y - halfH, vec.x + halfW, vec.y + halfH, 0xFFFFFFFF, true);
    DrawCircleAA(vec.x, vec.y, 2, 8, GetColor(255, 0, 0), true);

    stack->Pop();
}
