#pragma once
#include "../PhysicsComponent.h"
#include "../../graph/Graph.h"

class TitleWorldGroundComponent final : public PhysicsComponent
{
    std::unique_ptr<Graph> ground_graph_;
    std::unique_ptr<Graph> grasses_graph_;

public:
    explicit TitleWorldGroundComponent(int id);
    ~TitleWorldGroundComponent() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
