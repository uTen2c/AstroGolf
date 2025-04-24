#include "GameCamera.h"

#include <spdlog/spdlog.h>

Pos GameCamera::GetPos() const
{
    return pos_;
}

void GameCamera::SetPos(const Pos& pos)
{
    pos_ = pos;
}
