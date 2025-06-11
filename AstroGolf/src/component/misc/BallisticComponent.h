#pragma once

#include "../PhysicsComponent.h"

class BallisticComponent final : public PhysicsComponent
{
    std::deque<Vec2> trails_ = {};
    int trail_screen_;

public:
    explicit BallisticComponent(int id);
    ~BallisticComponent() override;

    void Draw(DrawStack* stack) override;
    void Update(float deltaTime) override;

    void Reset();
};
