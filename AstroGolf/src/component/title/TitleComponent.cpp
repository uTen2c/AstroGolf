#include "TitleComponent.h"

TitleComponent::TitleComponent(const int id)
    : Component(id),
      graph_(std::make_unique<Graph>("title.png", 1024, 1024))
{
}

TitleComponent::~TitleComponent() = default;

void TitleComponent::Draw(DrawStack* stack)
{
    stack->Push();
    graph_->Draw(*stack);
    stack->Pop();
}
