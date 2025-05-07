#pragma once
#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
public:
    Vec2 intersectingNormal;
    float radius = 32;
    
    explicit PlayerComponent(int id);

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
