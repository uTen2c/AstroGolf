#pragma once
#include <memory>
#include <string>

#include "../PlanetComponent.h"

class Graph;

class CommonPlanetComponent : public PlanetComponent
{
    std::string graph_id_;

public:
    CommonPlanetComponent(int id, float radius, std::string graphId);
    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
