#include "Component.h"

#include <spdlog/spdlog.h>

void Component::Init()
{
}

void Component::Update(float delta)
{
}

void Component::PostUpdate(float delta)
{
}

void Component::Draw(DrawStack* stack)
{
}

void Component::SetWorldPos(const Vec2& pos)
{
    const Vec2 parentPos = parent ? parent->GetWorldPos().pos : Vec2();
    transform.translate = pos.Copy().Sub(parentPos);
}

AbsolutePos Component::GetWorldPos() const
{
    std::shared_ptr<Component> parentPtr = parent;

    // ホントは条件分けせずにできたほうがいい
    if (!parentPtr)
    {
        return {.pos = transform.translate, .rot = transform.rotation};
    }

    std::vector<Transform> transforms = {};
    while (parentPtr != nullptr)
    {
        transforms.emplace_back(parentPtr->transform);
        parentPtr = parentPtr->parent;
    }

    Vec2 pos;
    auto rot = 0.0f;
    for (int i = static_cast<int>(transforms.size() - 1); i >= 0; i--)
    {
        const auto& t = transforms[i];
        const auto& rotated = t.translate.Rotated(rot);
        pos.Add(rotated);
        rot += t.rotation;
    }

    const auto& rotated = transform.translate.Rotated(rot);
    pos.Add(rotated);

    return {.pos = pos, .rot = rot};
}

int Component::GetId() const
{
    return id_;
}

Component Component::Copy(const int id) const
{
    auto copy = *this;
    copy.id_ = id;
    return copy;
}

void Component::ApplyDrawStack(DrawStack* stack) const
{
    const auto& [pos, rot] = GetWorldPos();
    stack->Translate(pos);
    stack->Rotate(rot);
}
