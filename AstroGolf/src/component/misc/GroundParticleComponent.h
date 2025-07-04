#pragma once
#include "../Component.h"

class GroundParticleComponent final : public Component
{
    float alive_time_ = 0;
    float frame_speed_ = 1.0f / 30.0f;

public:
    Vec2 normal;

    explicit GroundParticleComponent(int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
