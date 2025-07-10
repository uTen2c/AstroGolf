#pragma once
#include "Component.h"

class CameraComponent final : public Component
{
    bool dragging_ = false;
    Vec2 prev_pos_;
    int drag_start_pos_x_;
    int drag_start_pos_y_;

public:
    float zoom = 1.0f;
    float minZoomLevel = 0.5f;
    float maxZoomLevel = 2.0f;
    bool draggable = true;

    explicit CameraComponent(const int id)
        : Component(id), drag_start_pos_x_(0), drag_start_pos_y_(0)
    {
    }

    void Update(float delta) override;
private:
    void UpdatePos();
    void UpdateZoom();
};
