#include "GoalHoleComponent.h"

#include <spdlog/spdlog.h>

#include "../math/HoleCollider.h"
#include "../world/World.h"

GoalHoleComponent::GoalHoleComponent(const int id): PhysicsComponent(id)
{
    collider = std::make_unique<HoleCollider>();
    goal_collider_ = std::make_unique<RotatableBoxCollider>(60 - 8, 60 - 8, transform.rotation);
    hole_graph_ = std::make_unique<Graph>("hole.png", 128, 128);
    flag_graph_ = std::make_unique<Graph>("goal_flag.png", 128, 128);
}

void GoalHoleComponent::Update(float deltaTime)
{
    if (const auto hole = dynamic_cast<HoleCollider*>(collider.get()))
    {
        hole->rotation = transform.rotation;
    }

    PhysicsComponent::Update(deltaTime);

    CheckGoal(deltaTime);
}

void GoalHoleComponent::CheckGoal(const float deltaTime)
{
    goal_collider_->rotation = transform.rotation;

    const auto& player = world->GetPlayer();
    const auto& result = goal_collider_->
        Intersects(transform.translate, player->transform.translate, *player->collider);
    if (result.intersected)
    {
        check_duration_sec_ += deltaTime;
    } else
    {
        check_duration_sec_ = 0.0f;
    }

    if (check_duration_sec_ > 1.5 && !goaled_)
    {
        goaled_ = true;
        OnGoal();
        spdlog::info("Goal");
    }
}

void GoalHoleComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);
    hole_graph_->Draw(*stack);
    stack->Pop();

    // Debug
    // stack->Push();
    // transform.ApplyDrawStack(stack);
    // collider->Draw(stack);
    // stack->Pop();
}

void GoalHoleComponent::OnGoal()
{
}
