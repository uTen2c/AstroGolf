#pragma once

#include "../PhysicsComponent.h"

class BallisticComponent final : public PhysicsComponent
{
    std::vector<Vec2> trails_ = {};
    int trail_screen_;

public:
    bool shouldDraw = false;
    
    explicit BallisticComponent(int id);
    ~BallisticComponent() override;

    void Draw(DrawStack* stack) override;
    void DrawPoints(DrawStack* stack) const;
    void Update(float deltaTime) override;
    void CreatePoint();

    void Reset();
};
