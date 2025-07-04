#include "GroundParticleComponent.h"

#include <spdlog/spdlog.h>

#include "../../graph/Graphs.h"
#include "../../math/Math.h"
#include "../../random/Random.h"
#include "../../world/World.h"

GroundParticleComponent::GroundParticleComponent(const int id): Component(id)
{
    zIndex = 1101;
}

void GroundParticleComponent::Update(const float delta)
{
    Component::Update(delta);

    alive_time_ += delta;

    if (alive_time_ > 4 * frame_speed_)
    {
        world->RemoveComponent(this->GetId());
    }
}

void GroundParticleComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);
    
    const auto rotation = normal.SignedAngle({0, -1});
    stack->Rotate(rotation);
    
    stack->Push();
    stack->Translate(Vec2(0, -24).Rotate(rotation));
    

    const int frameIndex = std::min(static_cast<int>(floor(alive_time_ / frame_speed_)), 3);
    Graphs::landingParticles[frameIndex]->Draw(*stack);
    stack->Pop();
    stack->Pop();
}
