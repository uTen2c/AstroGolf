#pragma once
#include <memory>

#include "ColliderComponent.h"
#include "Component.h"
#include "../math/Collider.h"
#include "../math/RotatableBoxCollider.h"

class PhysicsComponent : public ColliderComponent
{
protected:
    bool intersecting_ = false;

public:
    std::unique_ptr<RotatableBoxCollider> goal_collider_;
    Vec2 velocity = {0, 0};

    float lastGravityPower = 0;
    std::vector<Vec2> gravitySources;

    float mass = 1;
    bool isStatic = false;

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
