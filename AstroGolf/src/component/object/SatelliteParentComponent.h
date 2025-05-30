#pragma once
#include "../Component.h"

class SatelliteParentComponent : public Component
{
public:
    explicit SatelliteParentComponent(const int id)
        : Component(id)
    {
    }

    void Update(float delta) override;
};
