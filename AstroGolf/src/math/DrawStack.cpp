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

void DrawStack::Translate(const Vector2d vector2d)
{
    const auto current = vec_stack_.top();
    Pop();
    vec_stack_.push(current.Add(vector2d));
}

Vector2d DrawStack::GetScreenPos() const
{
    Vector2d vec = {0, 0};
    auto copied = vec_stack_;
    while (!copied.empty())
    {
        vec = vec.Add(copied.top());
        copied.pop();
    }
    return vec;
}

Vector2d DrawStack::GetRelativePos() const
{
    return vec_stack_.top();
}
