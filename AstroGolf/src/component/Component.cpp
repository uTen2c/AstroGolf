#include "Component.h"

void Component::Update(double delta)
{
}

void Component::Draw(DrawStack* stack) const
{
}

int Component::GetId() const
{
    return id_;
}

Pos Component::GetPos() const
{
    return pos_;
}

void Component::SetPos(const Pos& pos)
{
    pos_ = pos;
}
