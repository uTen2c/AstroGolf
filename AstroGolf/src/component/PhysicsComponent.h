#pragma once
#include "Component.h"
#include "../math/Collider.h"

class PhysicsComponent : Component
{
    Vector2d velocity_;
    double mass_;

public:
    ~PhysicsComponent() override;

    void Update(double delta) override;

    void Draw(DrawStack* stack) const override;

    Collider GetCollider() const;

    void SetVelocity(const Vector2d& velocity);

    void AddVelocity(const Vector2d& velocity);

    double GetMass() const;

    void SetMass(const double& mass);
};
