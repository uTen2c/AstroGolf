#include "GoalHoleComponent.h"

#include <spdlog/spdlog.h>

#include "../graph/Graphs.h"
#include "../math/HoleCollider.h"
#include "../world/World.h"

GoalHoleComponent::GoalHoleComponent(const int id): PhysicsComponent(id)
{
    collider = std::make_unique<HoleCollider>();
    goal_collider_ = std::make_unique<RotatableBoxCollider>(60 - 8, 60 - 8, GetWorldPos().rot);
    zIndex = 1050;
}

void GoalHoleComponent::Update(const float deltaTime)
{
    if (const auto hole = dynamic_cast<HoleCollider*>(collider.get()))
    {
        hole->rotation = GetWorldPos().rot;
    }

    PhysicsComponent::Update(deltaTime);

    // spdlog::info("{}, {}", transform.translate.x, transform.translate.y);

    CheckGoal(deltaTime);
}

void GoalHoleComponent::CheckGoal(const float deltaTime)
{
    const auto& [pos, rot] = GetWorldPos();
    goal_collider_->rotation = rot;

    const auto& player = world->GetPlayer();
    const auto& result = goal_collider_->
        Intersects(pos, player->GetWorldPos().pos, *player->collider);
    if (result.intersected)
    {
        check_duration_sec_ += deltaTime;
    }
    else
    {
        check_duration_sec_ = 0.0f;
        goaled_ = false;
    }

    if (check_duration_sec_ > 0.5f && !goaled_)
    {
        goaled_ = true;
        OnGoal();
        spdlog::info("Goal");
    }
}

void GoalHoleComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);
    Graphs::goalHole->Draw(*stack);
    stack->Pop();
}

void GoalHoleComponent::OnGoal()
{
    world->OnGoal();
}
