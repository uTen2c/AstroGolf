#pragma once
#include <memory>
#include <string>

#include "../PlanetComponent.h"

class Graph;

class CommonPlanetComponent : public PlanetComponent
{
    // Editor
    bool rotationOriginDragging = false;
    Vec2 rotationOriginDragStartOffset;
    Vec2 rotationOriginDragStartMousePos;

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
    Vec2 rotationOriginOffset = {0, -radius - 30};

    CommonPlanetComponent(int id, float radius, std::string graphId);
    void Update(float deltaTime) override;
    void UpdateInPlay(float deltaTime);
    void UpdateSatellite(float deltaTime);
    void UpdateInEditor(float deltaTime);
    void Draw(DrawStack* stack) override;
};
