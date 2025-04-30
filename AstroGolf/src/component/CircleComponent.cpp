#include "CircleComponent.h"

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
    DrawCircleAA(pos.x, pos.y, radius, 16, 0xFFFFFFFF, true);

    stack->Pop();
}
