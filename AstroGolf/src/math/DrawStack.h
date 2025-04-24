#pragma once
#include <stack>

#include "Pos.h"

class DrawStack
{
    std::stack<Vector2d> vec_stack_ = {};
    Vector2d vector2d_ = {0, 0};

public:
    DrawStack();

    void Push();
    void Pop();

    void Translate(Vector2d vector2d);

    Vector2d GetScreenPos() const;

    Vector2d GetRelativePos() const;
};
