#include "BoxComponent.h"

#include <DxLib.h>

#include "../math/BoundingBox.h"

BoxComponent::BoxComponent(const int id, const float width, const float height): PhysicsComponent(id), width(width),
    height(height)
{
    collider = std::make_unique<BoundingBox>(width, height);
    isStatic = true;
}

void BoxComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    const auto halfW = width * 0.5f;
    const auto halfH = height * 0.5f;
    DrawBoxAA(vec.x - halfW, vec.y - halfH, vec.x + halfW, vec.y + halfH, 0xFFFFFFFF, true);
    DrawCircleAA(vec.x, vec.y, 2, 8, GetColor(255, 0, 0), true);

    stack->Pop();
}
