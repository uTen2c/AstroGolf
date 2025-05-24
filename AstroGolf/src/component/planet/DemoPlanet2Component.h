#pragma once
#include "../PlanetComponent.h"

class DemoPlanet2Component final : public PlanetComponent
{
    int graph_handle_;

public:
    explicit DemoPlanet2Component(int id);
    ~DemoPlanet2Component() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
