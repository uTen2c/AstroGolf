#pragma once
#include "../PlanetComponent.h"

class DemoPlanetComponent final : public PlanetComponent
{
    int graph_handle_;

public:
    explicit DemoPlanetComponent(int id);
    ~DemoPlanetComponent() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
