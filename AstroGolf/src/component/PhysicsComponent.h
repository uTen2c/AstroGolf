#pragma once
#include <memory>

#include "Component.h"
#include "../math/Collider.h"

class PhysicsComponent : public Component
{
protected:
    bool intersecting_ = false;

public:
    std::unique_ptr<Collider> collider;
    Vec2 velocity = {0, 0};
    Vec2 gravityVelocity = {0, 0};
    float gravity = 0.0f;
    float mass = 1;
    bool isStatic = false;

    explicit PhysicsComponent(int id);

    void Update(float deltaTime) override;

    void Draw(DrawStack* stack) override;

    void Move(const Vec2& delta);
};
