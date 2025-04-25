#pragma once
#include "Component.h"

class SimpleSquareComponent final : public Component
{
public:
    explicit SimpleSquareComponent(const int id)
        : Component(id)
    {
    }

    void Draw(DrawStack* stack) const override;
};
