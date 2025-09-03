#pragma once
#include <memory>
#include <string>

#include "../PlanetComponent.h"

class Graph;

class CommonPlanetComponent : public PlanetComponent
{
    // Editor
    bool rotation_origin_dragging_ = false;
    Vec2 rotation_origin_drag_start_offset_;
    Vec2 rotation_origin_drag_start_mouse_pos_;

    // Play
    bool origin_world_pos_initialized_ = false;
    Vec2 origin_world_pos_;
    float rotation_distance_;
    float rotation_angle_;

public:
    std::string graphId;
    float gravityMultiplier = 1.0f;
    bool isSatellite = false;
    float rotationSpeed = 0;
    Vec2 rotationOriginOffset = {0.0f, -radius - 30};

    CommonPlanetComponent(int id, float radius, std::string graphId);
    void Update(float deltaTime) override;
    void UpdateInPlay(float deltaTime);
    void UpdateSatellite(float deltaTime);
    void UpdateInEditor(float deltaTime);
    void Draw(DrawStack* stack) override;
};
