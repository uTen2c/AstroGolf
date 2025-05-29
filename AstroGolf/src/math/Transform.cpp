#include "Transform.h"

void Transform::ApplyDrawStack(DrawStack* stack) const
{
    stack->Translate(translate);
    stack->Scale(scale);
    stack->Rotate(rotation);
}
