#pragma once
#include "../math/Vec2.h"

class GameCamera
{
public:
    Vec2 translate = {0, 0};

    GameCamera() = default;
    GameCamera(const GameCamera&) = delete;
    GameCamera& operator=(const GameCamera&) = delete;
    GameCamera(GameCamera&&) = delete;
    GameCamera& operator=(GameCamera&&) = delete;

    ~GameCamera() = default;
};
