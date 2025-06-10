#pragma once
#include <memory>

#include "../math/DrawStack.h"
#include "../math/Transform.h"

class World;

struct AbsolutePos
{
    Vec2 pos;
    float rot;
};

class Component
{
    int id_;

public:
    Transform transform;
    World* world = nullptr;
    int zIndex = 1000;
    std::shared_ptr<Component> parent = nullptr;

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
    virtual void PostUpdate(float delta);
    virtual void Draw(DrawStack* stack);

    void SetWorldPos(const Vec2& pos);

    [[nodiscard]] AbsolutePos GetWorldPos() const;

    [[nodiscard]] int GetId() const;

protected:
    void ApplyDrawStack(DrawStack* stack) const;
};
