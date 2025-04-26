#pragma once
#include <memory>

#include "world/World.h"

class Game
{
    std::unique_ptr<World> world_ = std::make_unique<World>();

public:
    inline static int fps = 0;
    inline static float deltaTime = 0;

    Game();
    [[nodiscard]] World& GetWorld() const;
};
