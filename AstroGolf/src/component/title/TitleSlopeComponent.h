#pragma once
#include "../PhysicsComponent.h"
#include "../../math/Math.h"
#include "../../math/RotatableBoxCollider.h"

class TitleSlopeComponent final : public PhysicsComponent
{
public:
    explicit TitleSlopeComponent(const int id)
        : PhysicsComponent(id)
    {
        collider = std::make_unique<RotatableBoxCollider>(std::numbers::sqrt2 * 200, std::numbers::sqrt2 * 200,
                                                          45.0f * Math::deg_to_rad);
    }


    void Draw(DrawStack* stack) override;
};
