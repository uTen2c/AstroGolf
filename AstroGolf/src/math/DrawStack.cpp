#include "DrawStack.h"

DrawStack::DrawStack()
{
    vec_stack_.emplace(0, 0);
}

void DrawStack::Push()
{
    vec_stack_.emplace(0, 0);
}

void DrawStack::Pop()
{
    vec_stack_.pop();
}

void DrawStack::Translate(const Vec2 vector2d)
{
    auto current = vec_stack_.top();
    Pop();
    current.Add(vector2d);
    vec_stack_.push(current);
}

Vec2 DrawStack::GetScreenPos() const
{
    Vec2 vec;
    auto copied = vec_stack_;
    while (!copied.empty())
    {
        vec.Add(copied.top());
        copied.pop();
    }
    return vec;
}

Vec2 DrawStack::GetRelativePos() const
{
    return vec_stack_.top();
}
