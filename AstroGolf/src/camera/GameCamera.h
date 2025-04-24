#pragma once
#include "../math/Pos.h"

class GameCamera
{
    Pos pos_ = {{0, 0}, 0.0};

public:
    GameCamera() = default;
    GameCamera(const GameCamera&) = delete;
    GameCamera& operator=(const GameCamera&) = delete;
    GameCamera(GameCamera&&) = delete;
    GameCamera& operator=(GameCamera&&) = delete;

    ~GameCamera() = default;


    [[nodiscard]] Pos GetPos() const;
    void SetPos(const Pos& pos);
};
