#pragma once
#include "Component.h"

class SimpleSquareComponent final : public Component
{
public:
    SimpleSquareComponent(const int id, const Pos& pos)
        : Component(id, pos)
    {
    }

    void Draw(DrawStack* stack) const override;
};
