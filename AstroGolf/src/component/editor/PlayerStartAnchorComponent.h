#pragma once
#include "../ColliderComponent.h"
#include "../Component.h"

class PlayerStartAnchorComponent final : public ColliderComponent
{
public:
    explicit PlayerStartAnchorComponent(const int id);

    void Draw(DrawStack* stack) override;
};
