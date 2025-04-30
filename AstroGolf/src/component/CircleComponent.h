#pragma once
#include "PhysicsComponent.h"
#include "../math/CircleCollider.h"

class CircleComponent final : public PhysicsComponent
{
public:
    float radius;

    explicit CircleComponent(const int id, const float radius);

    void Draw(DrawStack* stack) override;
};
