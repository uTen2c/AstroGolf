#pragma once
#include "../PhysicsComponent.h"

class TitleWorldGroundComponent final : public PhysicsComponent
{
public:
    explicit TitleWorldGroundComponent(const int id)
        : PhysicsComponent(id)
    {
    }
};
