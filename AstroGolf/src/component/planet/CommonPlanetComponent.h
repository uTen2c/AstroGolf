#pragma once
#include <memory>
#include <string>

#include "../PlanetComponent.h"

class Graph;

class CommonPlanetComponent : public PlanetComponent
{

public:
    std::string graphId;
    float gravityMultiplier = 1.0f;
    CommonPlanetComponent(int id, float radius, std::string graphId);
    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
