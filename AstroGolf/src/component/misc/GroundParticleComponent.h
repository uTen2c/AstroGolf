#pragma once
#include "../Component.h"
#include "../PhysicsComponent.h"

class GroundParticleComponent : public Component
{
    float alive_time_ = 0;
    std::vector<float> angles_;
    float frame_speed_ = 1.0f / 30.0f;

public:
    Vec2 normal;
    float speed = 0;
    
    explicit GroundParticleComponent(const int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
