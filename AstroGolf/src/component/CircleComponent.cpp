﻿#include "CircleComponent.h"

#include <DxLib.h>

CircleComponent::CircleComponent(const int id, const float radius): PhysicsComponent(id), radius(radius)
{
    collider = std::make_unique<CircleCollider>(radius);
    isStatic = true;
}

void CircleComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto pos = stack->GetScreenPos();
    const auto scale = stack->GetScreenScale();
    const auto posnum = max(16 * scale.x, 16);
    DrawCircleAA(pos.x, pos.y, radius * scale.x, posnum, 0xFFFFFFFF, true);

    stack->Pop();
}
