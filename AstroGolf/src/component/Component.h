#pragma once
#include "../math/DrawStack.h"
#include "../math/Transform.h"

class Component
{
    int id_;

public:
    Transform transform;

    explicit Component(const int id): id_(id)
    {
    }

    virtual ~Component() = default;

    // デフォルト実装を使用
    Component(const Component&) = default;
    Component& operator=(const Component&) = default;
    Component(Component&&) noexcept = default;
    Component& operator=(Component&&) noexcept = default;

    virtual void Update(float delta);
    virtual void Draw(DrawStack* stack) const;

    [[nodiscard]] int GetId() const;
};
