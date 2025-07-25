﻿#pragma once
#include "PhysicsComponent.h"

class BoxComponent final : public PhysicsComponent
{
public:
    float width;
    float height;

    explicit BoxComponent(int id, float width, float height);
    ~BoxComponent() override;

    void Draw(DrawStack* stack) override;
};
