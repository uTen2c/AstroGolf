#include "StartTextComponent.h"

#include <valarray>

void StartTextComponent::Update(const float delta)
{
    duration_ += delta * 3;
    draw_offset_ = std::sin(duration_) * 8;
}

void StartTextComponent::Draw(DrawStack* stack)
{
    stack->Push();
    ApplyDrawStack(stack);
    stack->Translate({0.0f, draw_offset_});
    start_text_graph_->Draw(*stack);
    stack->Pop();
}
