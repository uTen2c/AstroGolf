#include "PhysicsComponent.h"

#include "../math/NullCollider.h"

PhysicsComponent::PhysicsComponent(const int id): Component(id)
{
    SetCollider(Collider(NullCollider()));
}

void PhysicsComponent::Update(const float delta)
{
    Component::Update(delta);
}

void PhysicsComponent::Draw(DrawStack* stack) const
{
    Component::Draw(stack);
}

Collider PhysicsComponent::GetCollider() const
{
    return collider_;
}

void PhysicsComponent::SetCollider(const Collider& collider)
{
    collider_ = collider;
}
