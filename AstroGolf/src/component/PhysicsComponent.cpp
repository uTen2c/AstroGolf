#include "PhysicsComponent.h"

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(double delta)
{
    Component::Update(delta);
}

void PhysicsComponent::Draw(DrawStack* stack) const
{
    Component::Draw(stack);
}

Collider PhysicsComponent::GetCollider() const
{
    
}

void PhysicsComponent::SetVelocity(const Vector2d& velocity)
{
}

void PhysicsComponent::AddVelocity(const Vector2d& velocity)
{
}

double PhysicsComponent::GetMass() const
{
}

void PhysicsComponent::SetMass(const double& mass)
{
}
