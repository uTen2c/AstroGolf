#pragma once
#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
public:
    explicit PlayerComponent(int id);

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
