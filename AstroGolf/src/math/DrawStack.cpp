#include "DrawStack.h"

DrawStack::DrawStack()
{
    vec_stack_.emplace(0, 0);
    scale_stack_.emplace(1, 1);
}

void DrawStack::Push()
{
    vec_stack_.emplace(0, 0);
    scale_stack_.emplace(1, 1);
}

void DrawStack::Pop()
{
    vec_stack_.pop();
    scale_stack_.pop();
}

void DrawStack::Translate(const Vec2 vector2d)
{
    auto& current = vec_stack_.top();
    current.Add(vector2d);
}

void DrawStack::Scale(const float scale)
{
    auto& current = scale_stack_.top();
    current.x = scale;
    current.y = scale;
}

void DrawStack::Scale(const Vec2 scale)
{
    auto& current = scale_stack_.top();
    current = scale;
}

void DrawStack::Rotate(const float angle)
{
    if (!angle_stack_.empty())
    {
        angle_stack_.pop();
    }
    angle_stack_.push(angle);
}

Vec2 DrawStack::GetScreenPos() const
{
    Vec2 vec;
    auto copiedVecStack = vec_stack_;
    auto copiedScaleStack = scale_stack_;
    while (!copiedVecStack.empty())
    {
        auto vec2 = copiedVecStack.top();
        auto scale = MergeScaleStack(copiedScaleStack);
        vec2.Mul(scale);
        vec.Add(vec2);
        copiedVecStack.pop();
        copiedScaleStack.pop();
    }
    return vec;
}

Vec2 DrawStack::GetRelativePos() const
{
    return vec_stack_.top();
}

Vec2 DrawStack::GetScreenScale() const
{
    return MergeScaleStack(scale_stack_);
}

float DrawStack::GetScreenRotate() const
{
    return MergeAngleStack(angle_stack_);
}

Vec2 DrawStack::MergeScaleStack(std::stack<Vec2> stack)
{
    Vec2 scale = {1, 1};
    while (!stack.empty())
    {
        scale.Mul(stack.top());
        stack.pop();
    }
    return scale;
}

float DrawStack::MergeAngleStack(std::stack<float> stack)
{
    float angle = 0;
    while (!stack.empty())
    {
        angle += stack.top();
        stack.pop();
    }
    return angle;
}
