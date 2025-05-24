#pragma once
#include <memory>

#include "world/World.h"

constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_HEIGHT = 720;

class Game
{
    std::unique_ptr<World> world_;

public:
    inline static int fps = 0;
    inline static float deltaTime = 0;

    Game();
    [[nodiscard]] World& GetWorld() const;

    void Update();

    template <class T, class... Args>
    void ChangeWorld(Args&&... args)
    {
        static_assert(std::is_base_of_v<World, T>, "T must inherit from World");
        world_ = std::make_unique<T>(std::forward<Args>(args)...);
    }
};
