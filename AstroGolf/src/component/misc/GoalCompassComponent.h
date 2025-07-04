#pragma once
#include "../Component.h"

class GoalCompassComponent final : public Component
{
    bool hidden_ = false;
    float compass_angle_ = 0;

public:
    int targetId = -1;
    explicit GoalCompassComponent(int id);

    void Update(float delta) override;
    void Draw(DrawStack* stack) override;
};
