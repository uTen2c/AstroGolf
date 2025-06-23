#include "TitleWorldGroundComponent.h"

#include "../../graph/Graphs.h"
#include "../../math/RotatableBoxCollider.h"

TitleWorldGroundComponent::TitleWorldGroundComponent(const int id): PhysicsComponent(id)
{
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
    Graphs::titleGroundGraph->Draw(*stack);
    Graphs::titleGrassesGraph->Draw(*stack);
    stack->Pop();
}
