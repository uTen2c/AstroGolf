#pragma once
#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
public:
    Vec2 intersectingNormal;
    float radius = 12;
    int shotCount = 0;
    bool isDragging = false;

    explicit PlayerComponent(int id);

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;

private:
    Vec2 drag_vector_;
    Vec2 screen_pos_;

    void UpdateShot();
    void UpdateDebugMove();
};
