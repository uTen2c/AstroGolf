#include "TitleWorldGroundComponent.h"

#include "../../math/RotatableBoxCollider.h"

TitleWorldGroundComponent::TitleWorldGroundComponent(const int id): PhysicsComponent(id)
{
    ground_graph_ = std::make_unique<Graph>("title_ground.png", 2048, 2048);
    grasses_graph_ = std::make_unique<Graph>("grasses.png", 2048, 2048);
    collider = std::make_unique<RotatableBoxCollider>(4000, 600, 0);
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
    stack->Translate({0, -300});
    ground_graph_->Draw(*stack);
    grasses_graph_->Draw(*stack);
    stack->Pop();
}
