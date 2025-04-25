#pragma once
#include "DrawStack.h"
#include "Vec2.h"

class Transform
{
public:
    Vec2 translate = {0, 0};
    float rotation = 0;
    Vec2 scale = {1, 1};

    void ApplyDrawStack(DrawStack* stack) const;
};
