#pragma once
#include "Component.h"

class CameraComponent final : public Component
{
    bool dragging_ = false;
    Vec2 prev_pos;
    int drag_start_pos_x_;
    int drag_start_pos_y_;
    
public:
    explicit CameraComponent(const int id)
        : Component(id)
    {
    }

    void Update(float delta) override;
};
