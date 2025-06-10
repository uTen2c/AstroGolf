#pragma once
#include "../PhysicsComponent.h"

class BallisticComponent final : public PhysicsComponent
{
public:
    explicit BallisticComponent(const int id)
        : PhysicsComponent(id)
    {
        collider = std::make_unique<CircleCollider>(5);
    }
};
