#pragma once
#include "../PhysicsComponent.h"
#include "../../graph/Graph.h"

class TitleWorldGroundComponent final : public PhysicsComponent
{
public:
    explicit TitleWorldGroundComponent(int id);
    ~TitleWorldGroundComponent() override;

    void Update(float deltaTime) override;
    void Draw(DrawStack* stack) override;
};
