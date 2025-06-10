#pragma once
#include "../Component.h"
#include "../../graph/Graph.h"

class StartTextComponent final : public Component
{
    std::unique_ptr<Graph> start_text_graph_;
    float duration_ = 0;
    float draw_offset_ = 0;

public:
    explicit StartTextComponent(const int id)
        : Component(id)
    {
        start_text_graph_ = std::make_unique<Graph>("title/start.png", 256, 256);
    }

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
