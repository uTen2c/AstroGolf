#include "BoxComponent.h"

#include <DxLib.h>

#include "../math/BoundingBox.h"

BoxComponent::BoxComponent(const int id, const float width, const float height): PhysicsComponent(id), width(width),
    height(height)
{
    SetCollider(BoundingBox({}, width, height));
}

void BoxComponent::Update(const float delta)
{
    PhysicsComponent::Update(delta);
}

void BoxComponent::Draw(DrawStack* stack) const
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    const auto half_w = width * 0.5f;
    const auto half_h = height * 0.5f;
    DrawBoxAA(vec.x - half_w, vec.y - half_h, vec.x + half_w, vec.y + half_h, 0xFFFFFFFF, true);

    stack->Pop();
}
