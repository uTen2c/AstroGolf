#pragma once
#include "PhysicsComponent.h"
#include "../graph/Graph.h"

class GoalHoleComponent : public PhysicsComponent
{
    float check_duration_sec_;
    bool goaled_ = false;
    
public:
    explicit GoalHoleComponent(int id);

    void Update(float deltaTime) override;
    void CheckGoal(float deltaTime);
    void Draw(DrawStack* stack) override;

    virtual void  OnGoal();
};
