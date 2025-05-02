#pragma once
#include "PhysicsComponent.h"
#include "../math/DrawStack.h"

class RotatableBoxComponent final : public PhysicsComponent
{
public:
    float width;
    float height;

    explicit RotatableBoxComponent(int id, float width, float height);
    ~RotatableBoxComponent() override = default;

    void Update(float delta) override;

    void Draw(DrawStack* stack) override;
};
