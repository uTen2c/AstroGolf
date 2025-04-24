#pragma once
#include "../math/DrawStack.h"
#include "../math/Pos.h"

class Component
{
    int id_;
    Pos pos_;

public:
    Component(const int id, const Pos& pos)
        : id_(id),
          pos_(pos)
    {
    }

    virtual ~Component() = default;
    virtual void Update(double delta);
    virtual void Draw(DrawStack* stack) const;

    [[nodiscard]] int GetId() const;

    [[nodiscard]] Pos GetPos() const;

    void SetPos(const Pos& pos);
};
