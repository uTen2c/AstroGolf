#pragma once
#include "PhysicsComponent.h"
#include "../math/CircleCollider.h"

class PlayerComponent final : public PhysicsComponent
{
    CircleCollider circle_collider_ = CircleCollider({0, 0}, 16);
    
public:
    explicit PlayerComponent(const int id);

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) const override;
};
