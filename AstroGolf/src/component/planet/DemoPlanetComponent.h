#pragma once
#include "../PlanetComponent.h"
#include "../../graph/Graph.h"

class DemoPlanetComponent final : public PlanetComponent
{
    std::unique_ptr<Graph> graph_;

public:
    explicit DemoPlanetComponent(int id);
    ~DemoPlanetComponent() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
