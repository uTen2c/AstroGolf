#pragma once
#include <memory>

#include "../Component.h"
#include "../../graph/Graph.h"

class TitleComponent final : public Component
{
    std::unique_ptr<Graph> graph_;

public:
    explicit TitleComponent(int id);
    ~TitleComponent() override;

    void Draw(DrawStack* stack) override;
};
