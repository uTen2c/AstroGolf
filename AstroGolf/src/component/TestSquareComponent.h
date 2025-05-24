#pragma once
#include <memory>

#include "Component.h"
#include "../graph/Graph.h"

class TestSquareComponent final : public Component
{
    std::unique_ptr<Graph> graph_;
    float scale_x_ = 1.0f;
    float scale_y_ = 1.0f;

public:
    explicit TestSquareComponent(int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
