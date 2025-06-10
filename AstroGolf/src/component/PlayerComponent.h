#pragma once
#include "PhysicsComponent.h"

class PlayerComponent final : public PhysicsComponent
{
    bool can_shot_ = true;
    Transform shot_start_transform_ = {};
    std::deque<Vec2> trails_ = {};

public:
    Vec2 intersectingNormal;
    float radius = 12;
    int shotCount = 0;
    bool isDragging = false;

    explicit PlayerComponent(int id);

    void Update(float deltaTime) override;
    void UpdateMovement(float deltaTime) override;
    void Draw(DrawStack* stack) override;

    void Respawn();

private:
    Vec2 drag_vector_;
    Vec2 screen_pos_;

    void UpdateShot();
    void UpdateDebugMove();

    void DrawTrail(DrawStack* stack);
};
