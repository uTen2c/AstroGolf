#include "SimpleSquareComponent.h"

#include <DxLib.h>

void SimpleSquareComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    DrawBoxAA(vec.x - 1, vec.y - 1, vec.x + 1, vec.y + 1, 0xFFFFFFFF, true);

    stack->Pop();
}
