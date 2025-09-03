#include "PlayerStartAnchorComponent.h"

#include "../../Game.h"
#include "../../graph/Graphs.h"

PlayerStartAnchorComponent::PlayerStartAnchorComponent(const int id)
    : ColliderComponent(id)
{
    collider = std::make_unique<CircleCollider>(12.0f);
}

void PlayerStartAnchorComponent::Draw(DrawStack* stack)
{
    if (!Game::debugEnabled && Game::instance->GetWorld().GetType() != WorldType::Editor)
    {
        return;
    }

    stack->Push();
    ApplyDrawStack(stack);
    Graphs::editorPlayerStartAnchor->Draw(*stack);
    stack->Pop();
}
