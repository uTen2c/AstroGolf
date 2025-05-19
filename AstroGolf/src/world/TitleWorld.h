#pragma once
#include "World.h"

class TitleWorld final : public World
{
public:
    TitleWorld();
    void Update(const float& deltaTime) override;
    [[nodiscard]] WorldType GetType() const override;
};
