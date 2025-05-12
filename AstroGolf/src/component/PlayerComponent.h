#pragma once
#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
public:
    Vec2 intersectingNormal;
    float radius = 16;
    
    explicit PlayerComponent(int id);

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
