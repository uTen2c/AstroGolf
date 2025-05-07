#pragma once
#include "PhysicsComponent.h"

class PlanetComponent : public PhysicsComponent
{
public:
    float platnetGravity;
    float radius;

    explicit PlanetComponent(int id, float radius);

    void Update(float deltaTime) override;

    void Draw(DrawStack* stack) override;
private:
    float GetPlayerDistance()const;
};
