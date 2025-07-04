#include "GroundParticleComponent.h"

#include <spdlog/spdlog.h>

#include "../../graph/Graphs.h"
#include "../../math/Math.h"
#include "../../random/Random.h"
#include "../../world/World.h"

GroundParticleComponent::GroundParticleComponent(const int id): Component(id)
{
    zIndex = 1101;

    // angles_.resize(3);
    // for (int i = 0; i < 3; ++i)
    // {
    //     const auto rad = Math::Lerp(-90, 90, Random::GetInstance().Float()) * Math::deg_to_rad;
    //     angles_.emplace_back(rad);
    // }
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
    spdlog::info("frameIndex {}", frameIndex);
    Graphs::landingParticles[frameIndex]->Draw(*stack);
    // for (float angle : angles_)
    // {
    //     stack->Push();
    //
    //     const auto dir = normal.Rotated(angle);
    //     const auto lineStopVec = dir * speed * 2;
    //
    //     stack->Translate(dir * (alive_time_ * speed * 60));
    //
    //     const auto pos = stack->GetScreenPos();
    //     const auto scale = stack->GetScreenScale();
    //     DrawLineAA(pos.x - lineStopVec.x, pos.y - lineStopVec.y, pos.x + lineStopVec.x, pos.y + lineStopVec.y,
    //                GetColor(16, 89, 14), 4);
    //
    //     stack->Pop();
    // }
    stack->Pop();
    stack->Pop();
}
