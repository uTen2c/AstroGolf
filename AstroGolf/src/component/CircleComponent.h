#pragma once
#include "PhysicsComponent.h"

class CircleComponent final : public PhysicsComponent
{
public:
    float radius;

    explicit CircleComponent(const int id, const float radius);

    void Draw(DrawStack* stack) override;
};
