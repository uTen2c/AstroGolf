#pragma once
#include <memory>

#include "ColliderComponent.h"
#include "Component.h"
#include "../math/collider/Collider.h"
#include "../math/collider/RotatableBoxCollider.h"

class PhysicsComponent : public ColliderComponent
{
protected:
    bool intersecting_ = false;

public:
    std::unique_ptr<RotatableBoxCollider> goal_collider;
    Vec2 velocity = {0, 0};

    float last_gravity_power = 0;
    std::vector<Vec2> gravity_sources;

    float mass = 1;
    bool is_static = false;

    explicit PhysicsComponent(const int id)
        : ColliderComponent(id)
    {
    }

    void Update(float deltaTime) override;
    virtual void UpdateMovement(float deltaTime);
    void PostUpdate(float deltaTime) override;

    void Draw(DrawStack* stack) override;

    void Move(const Vec2& delta);

    [[nodiscard]] Vec2 GetMergedGravityVelocity() const;

    void CalcGravity();

protected:
    // 現在コライダーがBoundingBoxだと機能しない
    virtual void OnCollide(PhysicsComponent* other, const IntersectingResult& result);
};
