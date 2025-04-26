#pragma once
#include "Component.h"
#include "../math/Collider.h"

class PhysicsComponent : public Component
{
    Collider collider_;

public:
    Vec2 velocity = {0, 0};
    float mass = 1;

    explicit PhysicsComponent(int id);

    void Update(float delta) override;

    void Draw(DrawStack* stack) const override;

    [[nodiscard]] Collider GetCollider() const;

    void SetCollider(const Collider& collider);

    void Move(const Vec2& delta);

private:
    void CheckCollision();
};
