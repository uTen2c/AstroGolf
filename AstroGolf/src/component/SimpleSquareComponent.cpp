#include "SimpleSquareComponent.h"

#include <DxLib.h>

void SimpleSquareComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);

    const auto vec = stack->GetScreenPos();
    DrawBoxAA(vec.x - 2, vec.y - 2, vec.x + 2, vec.y + 2, GetColor(255, 0, 0), true);

    stack->Pop();
}
