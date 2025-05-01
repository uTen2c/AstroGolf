#pragma once
#include "PhysicsComponent.h"

class BoxComponent final : public PhysicsComponent
{
    int ps_handle_;
    
public:
    float width;
    float height;

    explicit BoxComponent(int id, float width, float height);
    ~BoxComponent() override;

    void Draw(DrawStack* stack) override;
};
