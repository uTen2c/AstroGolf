#pragma once
#include <stack>

#include "Pos.h"

class DrawStack
{
    std::stack<Vec2> vec_stack_ = {};
    Vec2 vector2d_ = {0, 0};

public:
    DrawStack();

    void Push();
    void Pop();

    void Translate(Vec2 vector2d);

    [[nodiscard]] Vec2 GetScreenPos() const;

    [[nodiscard]] Vec2 GetRelativePos() const;
};
