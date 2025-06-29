#pragma once
#include "Component.h"
#include "../math/NullCollider.h"

class Collider;

class ColliderComponent : public Component
{
public:
    std::unique_ptr<Collider> collider;

    explicit ColliderComponent(int id);
    ~ColliderComponent() override;
    void Init() override;
    void Update(float delta) override;
    void PostUpdate(float delta) override;
    void Draw(DrawStack* stack) override;
};
