#include "SimpleSquareComponent.h"

#include <DxLib.h>

void SimpleSquareComponent::Draw(DrawStack* stack) const
{
    stack->Push();
    stack->Translate(GetPos().GetPoint());

    const auto vec = stack->GetScreenPos();
    DrawBoxAA(vec.GetX() - 1, vec.GetY() - 1, vec.GetX() + 1, vec.GetY() + 1, 0xFFFFFFFF, true);

    stack->Pop();
}
