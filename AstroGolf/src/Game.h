#pragma once
#include <memory>

#include "Device.h"
#include "world/World.h"

constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_HEIGHT = 720;

enum class TransitionMode
{
    Instant,
    Slide,
    Circle,
};

class Game
{
    inline static float max_transition_delta_ = 1.f;
    inline static Device device_;

    std::unique_ptr<World> world_;
    bool changing_ = false;
    bool world_changed_ = false;
    float last_transition_progress_ = 0;
    float transition_delta_ = 0;
    std::unique_ptr<World> transition_world_;
    TransitionMode transition_mode_ = TransitionMode::Circle;

public:
    inline static std::unique_ptr<Game> instance = nullptr;
    inline static bool started = false;
    inline static int fps = 0;
    inline static float deltaTime = 0;
    inline static bool shouldShutdown = false;
    inline static bool uiHidden = false;
    inline static bool debugEnabled = false;
    inline static float soundVolume = 1.0f;

    bool isPaused = false;

    Game();
    [[nodiscard]] World& GetWorld() const;

    void Update();
    void UpdateTransition(float delta);

    template <class T, class... Args>
    void ChangeWorld(Args&&... args)
    {
        static_assert(std::is_base_of_v<World, T>, "T must inherit from World");
        changing_ = true;
        transition_mode_ = TransitionMode::Instant;
        transition_world_ = std::make_unique<T>(std::forward<Args>(args)...);
        isPaused = false;
    }

    template <class T, class... Args>
    void ChangeWorldWithTransition(const TransitionMode mode, Args&&... args)
    {
        ChangeWorldWithTransition(mode, std::make_unique<T>(std::forward<Args>(args)...));
    }

    void ChangeWorldWithTransition(const TransitionMode mode, std::unique_ptr<World> world)
    {
        isPaused = true;
        transition_mode_ = mode;
        transition_world_ = std::move(world);
        changing_ = true;
        transition_delta_ = 0;
        last_transition_progress_ = 0;
        world_changed_ = false;
        isPaused = false;
    }

    [[nodiscard]] static Device& Device();

private:
    [[nodiscard]] static float GetTransitionProgress(float elapsedTimeMs, float durationMs);
};
