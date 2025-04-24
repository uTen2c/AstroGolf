#pragma once
#include <memory>

#include "world/World.h"

class Game
{
    std::unique_ptr<World> world_ = std::make_unique<World>();

public:
    Game();
    ~Game() = default;
    [[nodiscard]] World& GetWorld() const;
};
