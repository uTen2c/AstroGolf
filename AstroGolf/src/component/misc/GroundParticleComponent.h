#pragma once
#include "../Component.h"
#include "../PhysicsComponent.h"

class GroundParticleComponent : public PhysicsComponent
{
    float alive_time_ = 0;
    std::vector<float> angles_;

public:
    Vec2 normal;
    float speed = 0;
    
    explicit GroundParticleComponent(const int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
