#pragma once
#include "PhysicsComponent.h"

class BoxComponent final : public PhysicsComponent
{
public:
    float width;
    float height;

    explicit BoxComponent(const int id, const float width, const float height);

    void Update(float delta) override;
    void Draw(DrawStack* stack) const override;
};
