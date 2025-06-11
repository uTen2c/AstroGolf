#pragma once
#include "PhysicsComponent.h"

class PlanetComponent : public PhysicsComponent
{
public:
    float planetGravity;
    float radius;

    explicit PlanetComponent(int id, float radius);

    void Update(float deltaTime) override;

    void Draw(DrawStack* stack) override;

    void ApplyGravity(PhysicsComponent* component) const;

private:
    [[nodiscard]] float GetPlayerDistance(const PhysicsComponent* component) const;
};
