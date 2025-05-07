#pragma once
#include <stack>

#include "Vec2.h"

class DrawStack
{
    std::stack<Vec2> vec_stack_ = {};
    std::stack<Vec2> scale_stack_ = {};
    Vec2 vector2d_ = {0, 0};

public:
    DrawStack();

    void Push();
    void Pop();

    void Translate(Vec2 vector2d);
    
    void Scale(float scale);
    void Scale(Vec2 scale);

    [[nodiscard]] Vec2 GetScreenPos() const;

    [[nodiscard]] Vec2 GetRelativePos() const;
    
    [[nodiscard]] Vec2 GetScreenScale() const;

private:
    static Vec2 MergeScaleStack(std::stack<Vec2> stack);
};
