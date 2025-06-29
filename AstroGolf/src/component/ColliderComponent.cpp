#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(const int id): Component(id)
{
    collider = std::make_unique<NullCollider>();
}

ColliderComponent::~ColliderComponent()
{
}

void ColliderComponent::Init()
{
    Component::Init();
}

void ColliderComponent::Update(float delta)
{
    Component::Update(delta);
}

void ColliderComponent::PostUpdate(float delta)
{
    Component::PostUpdate(delta);
}

void ColliderComponent::Draw(DrawStack* stack)
{
    Component::Draw(stack);
}
