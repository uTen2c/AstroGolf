#include "TitleWorldGroundComponent.h"

#include "../../math/RotatableBoxCollider.h"

TitleWorldGroundComponent::TitleWorldGroundComponent(const int id): PhysicsComponent(id)
{
    ground_graph_ = std::make_unique<Graph>("title_ground.png", 2048, 2048);
    collider = std::make_unique<RotatableBoxCollider>(4000, 200, 0);
    zIndex = 100;
}

TitleWorldGroundComponent::~TitleWorldGroundComponent()
{
}

void TitleWorldGroundComponent::Update(float deltaTime)
{
    PhysicsComponent::Update(deltaTime);
}

void TitleWorldGroundComponent::Draw(DrawStack* stack)
{
    stack->Push();
    transform.ApplyDrawStack(stack);
    stack->Translate({-1024, -1024 - 100});
    ground_graph_->Draw(*stack);
    stack->Pop();
}
