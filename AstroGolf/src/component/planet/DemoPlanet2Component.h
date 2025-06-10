#pragma once
#include "../PlanetComponent.h"
#include "../../graph/Graph.h"

class DemoPlanet2Component final : public PlanetComponent
{
    std::unique_ptr<Graph> graph_;

public:
    explicit DemoPlanet2Component(int id);
    ~DemoPlanet2Component() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
