#include "SatelliteParentComponent.h"

#include <complex>

#include "../../math/Math.h"

namespace
{
    static float t = 0.0f;
}

void SatelliteParentComponent::Update(const float delta)
{
    Component::Update(delta);

    transform.rotation += 90.0f * Math::deg_to_rad * delta;
    // const auto y = std::sin(t);
    // t += delta;

    // transform.translate.y = y * 100.0f;
    // transform.rotation = y * 100.0f * Math::deg_to_rad;
}
