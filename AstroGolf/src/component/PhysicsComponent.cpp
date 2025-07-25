#include "PhysicsComponent.h"

#include <spdlog/spdlog.h>

#include "PlanetComponent.h"
#include "../math/BoundingBox.h"
#include "../math/CircleCollider.h"
#include "../math/HoleCollider.h"
#include "../math/NullCollider.h"
#include "../math/RotatableBoxCollider.h"
#include "../world/TitleWorld.h"
#include "../world/World.h"

namespace
{
    [[maybe_unused]] float GetColliderRadiusX(const Collider* collider)
    {
        if (const auto bb = dynamic_cast<const BoundingBox*>(collider))
        {
            return bb->width * 0.5f;
        }
        if (const auto cc = dynamic_cast<const CircleCollider*>(collider))
        {
            return cc->radius;
        }
        return 0;
    }

    [[maybe_unused]] float GetColliderRadiusY(const Collider* collider)
    {
        if (const auto bb = dynamic_cast<const BoundingBox*>(collider))
        {
            return bb->height * 0.5f;
        }
        if (const auto cc = dynamic_cast<const CircleCollider*>(collider))
        {
            return cc->radius;
        }
        return 0;
    }
}

void PhysicsComponent::Update(float deltaTime)
{
    ColliderComponent::Update(deltaTime);
    CalcGravity();
}

void PhysicsComponent::UpdateMovement(const float deltaTime)
{
    // if (velocity.Length() > 0)
    // {
    auto copied = velocity;
    // copied.Normalize();
    // copied.Mul(min(velocity.Length(), 500.0f));
    copied.Mul(deltaTime);

    auto gravityVec = GetMergedGravityVelocity();
    lastGravityPower = gravityVec.Length();
    copied.Add(gravityVec.Mul(deltaTime));

    // if (gravity > 0 && !intersecting_)
    // {
    // velocity.y += gravity * Game::deltaTime;
    // }

    Move(copied);

    velocity.Add(gravityVec);
}

void PhysicsComponent::PostUpdate(const float deltaTime)
{
    gravitySources.clear();
}

void PhysicsComponent::Draw(DrawStack* stack)
{
    ColliderComponent::Draw(stack);
}

void PhysicsComponent::Move(const Vec2& delta)
{
    const auto selfWorldPos = GetWorldPos();
    const auto nearbyComponents = world->GetNearbyPhysicsComponents(selfWorldPos.pos, 10000); // FIXME radius
    // auto moved = Vec2(transform.translate.x, transform.translate.y);
    auto moved = Vec2(selfWorldPos.pos.x, selfWorldPos.pos.y);
    Vec2 normal;

    if (const auto selfCc = dynamic_cast<CircleCollider*>(collider.get()))
    {
        moved.Add(delta);

        auto inverted = false;


        // ２回 for 回すのはきれいじゃない
        for (const auto& nearbyComponent : nearbyComponents)
        {
            if (nearbyComponent == this)
            {
                continue;
            }

            // 弾道とプレイヤーは互いに干渉しない
            if (
                dynamic_cast<PlayerComponent*>(this) && dynamic_cast<BallisticComponent*>(nearbyComponent) ||
                dynamic_cast<BallisticComponent*>(this) && dynamic_cast<PlayerComponent*>(nearbyComponent) ||
                dynamic_cast<BallisticComponent*>(this) && dynamic_cast<BallisticComponent*>(nearbyComponent)
            )
            {
                continue;
            }

            const auto& worldPos = nearbyComponent->GetWorldPos();

            // ゴールホールは特殊な処理をするので、先に判定する
            if (const auto hole = dynamic_cast<HoleCollider*>(nearbyComponent->collider.get()))
            {
                if (
                    const auto areaResult = hole->IntersectsArea(worldPos.pos, moved, *selfCc);
                    !areaResult.intersected
                )
                {
                    continue;
                }

                inverted = true;

                const auto result = selfCc->Intersects(moved, worldPos.pos, *hole);
                if (!result.intersected)
                {
                    continue;
                }

                normal = result.normal;

                const auto diff = selfCc->radius - moved.Distance(result.point);
                auto negVec = normal;
                negVec.Mul(diff);
                moved.Add(negVec);

                OnCollide(nearbyComponent, result);

                break;
            }
        }

        for (const auto& nearbyComponent : nearbyComponents)
        {
            if (inverted)
            {
                break;
            }
            if (nearbyComponent == this)
            {
                continue;
            }

            if (dynamic_cast<HoleCollider*>(nearbyComponent->collider.get()))
            {
                continue;
            }

            // 弾道とプレイヤーは互いに干渉しない
            if (
                dynamic_cast<PlayerComponent*>(this) && dynamic_cast<BallisticComponent*>(nearbyComponent) ||
                dynamic_cast<BallisticComponent*>(this) && dynamic_cast<PlayerComponent*>(nearbyComponent) ||
                dynamic_cast<BallisticComponent*>(this) && dynamic_cast<BallisticComponent*>(nearbyComponent)
            )
            {
                continue;
            }

            const auto& worldPos = nearbyComponent->GetWorldPos();

            if (const auto cc = dynamic_cast<CircleCollider*>(nearbyComponent->collider.get()))
            {
                const auto& result = cc->Intersects(worldPos.pos, moved, *selfCc);
                if (!result.intersected)
                {
                    continue;
                }

                const auto otherPos = worldPos.pos;
                const auto distance = otherPos.Distance(moved);

                auto negVec = moved;
                negVec.Sub(otherPos);
                negVec.Normalize();

                const auto diff = selfCc->radius + cc->radius - distance;
                negVec.Mul(diff);

                moved.Add(negVec);

                normal = negVec.Normalized();

                OnCollide(nearbyComponent, result);

            }
            if (const auto bb = dynamic_cast<BoundingBox*>(nearbyComponent->collider.get()))
            {
                const auto otherPos = worldPos.pos;
                const auto x1 = otherPos.x + bb->GetLeft();
                const auto x2 = otherPos.x + bb->GetRight();
                const auto y1 = otherPos.y + bb->GetTop();
                const auto y2 = otherPos.y + bb->GetBottom();
                const auto r = selfCc->radius;

                // Top floor
                if (moved.x > x1 && moved.x < x2 && moved.y + r >= y1 && moved.y < y2)
                {
                    moved.y = y1 - r;
                    normal = {0, -1};
                    continue;
                }
                // Bottom floor
                if (moved.x > x1 && moved.x < x2 && moved.y > y1 && moved.y - r <= y2)
                {
                    moved.y = y2 + r;
                    normal = {0, 1};
                    continue;
                }
                // Left wall
                if (moved.x > x1 && moved.x - r <= x2 && moved.y > y1 && moved.y < y2)
                {
                    moved.x = x2 + r;
                    normal = {1, 0};
                    continue;
                }
                // Right wall
                if (moved.x + r >= x1 && moved.x < x2 && moved.y > y1 && moved.y < y2)
                {
                    moved.x = x1 - r;
                    normal = {-1, 0};
                    continue;
                }

                const Vec2 topLeft = {x1, y1};
                const Vec2 topRight = {x2, y1};
                const Vec2 bottomLeft = {x1, y2};
                const Vec2 bottomRight = {x2, y2};

                Vec2 hitCorner;

                if (selfCc->Contains(moved, topLeft))
                {
                    hitCorner = topLeft;
                }
                else if (selfCc->Contains(moved, topRight))
                {
                    hitCorner = topRight;
                }
                else if (selfCc->Contains(moved, bottomLeft))
                {
                    hitCorner = bottomLeft;
                }
                else if (selfCc->Contains(moved, bottomRight))
                {
                    hitCorner = bottomRight;
                }
                else
                {
                    continue;
                }

                auto negVec = moved;
                negVec.Sub(hitCorner);
                negVec.Normalize();

                normal = negVec;

                const auto diff = selfCc->radius - hitCorner.Distance(moved);
                negVec.Mul(diff);

                moved.Add(negVec);
            }
            if (const auto rbc = dynamic_cast<RotatableBoxCollider*>(nearbyComponent->collider.get()))
            {
                const auto& result = rbc->Intersects(worldPos.pos, moved, *selfCc);
                if (!result.intersected)
                {
                    continue;
                }

                normal = result.normal;
                const auto diff = rbc->Contains(worldPos.pos, moved)
                                      ? selfCc->radius + moved.Distance(result.point)
                                      : selfCc->radius - moved.Distance(result.point);

                auto negVec = normal;
                negVec.Mul(diff);
                moved.Add(negVec);
                OnCollide(nearbyComponent, result);
            }
        }
    }

    if (dynamic_cast<BoundingBox*>(collider.get()))
    {
        if (delta.x != 0.0f)
        {
            moved.x += delta.x;

            for (const auto& nearbyComponent : nearbyComponents)
            {
                if (nearbyComponent == this)
                {
                    continue;
                }

                const auto& worldPos = nearbyComponent->GetWorldPos();

                const auto& otherCollider = nearbyComponent->collider;
                if (const BoundingBox* bb = dynamic_cast<BoundingBox*>(otherCollider.get()))
                {
                    if (collider->Intersects(moved, worldPos.pos, *bb).intersected)
                    {
                        if (delta.x > 0)
                        {
                            const auto otherEdgeX = worldPos.pos.x - bb->width / 2;
                            moved.x = otherEdgeX - GetColliderRadiusX(collider.get());
                        }
                        else
                        {
                            const auto otherEdgeX = worldPos.pos.x + bb->width / 2;
                            moved.x = otherEdgeX + GetColliderRadiusX(collider.get());
                        }
                    }
                }
                else if (const CircleCollider* cc = dynamic_cast<CircleCollider*>(otherCollider.get()))
                {
                    //
                }
            }
        }

        if (delta.y != 0.0f)
        {
            moved.y += delta.y;

            for (const auto& nearbyComponent : nearbyComponents)
            {
                if (nearbyComponent == this)
                {
                    continue;
                }

                const auto& worldPos = nearbyComponent->GetWorldPos();

                const auto& otherCollider = nearbyComponent->collider;
                if (const BoundingBox* bb = dynamic_cast<BoundingBox*>(otherCollider.get()))
                {
                    if (collider->Intersects(moved, worldPos.pos, *bb).intersected)
                    {
                        if (delta.y > 0)
                        {
                            const auto otherEdgeY = worldPos.pos.y - bb->height / 2;
                            moved.y = otherEdgeY - GetColliderRadiusY(collider.get());
                        }
                        else
                        {
                            const auto otherEdgeY = worldPos.pos.y + bb->height / 2;
                            moved.y = otherEdgeY + GetColliderRadiusY(collider.get());
                        }
                    }
                }
                else if (const CircleCollider* cc = dynamic_cast<CircleCollider*>(otherCollider.get()))
                {
                    //
                }
            }
        }
    }


    intersecting_ = normal.Length() > 0;
    if (const auto player = dynamic_cast<PlayerComponent*>(this))
    {
        player->intersectingNormal = normal;
    }

    if (intersecting_)
    {
        const auto e = 0.75f;
        const auto gravityVec = GetMergedGravityVelocity();
        const auto mergedVec = velocity.Copy().Add(gravityVec);
        const auto a = mergedVec.Copy().Neg().Dot(normal);
        auto r = mergedVec.Copy().Add(normal.Copy().Mul(a * 2).Mul(e));
        r.Sub(gravityVec);
        velocity = r;
    }

    if (selfWorldPos.pos.Distance(moved) == 0.0f)
    {
        return;
    }

    if (!isStatic)
    {
        SetWorldPos(moved);
    }
}

Vec2 PhysicsComponent::GetMergedGravityVelocity() const
{
    Vec2 vec;
    for (const auto& gravitySource : gravitySources)
    {
        vec.Add(gravitySource);
    }
    return vec;
}

void PhysicsComponent::CalcGravity()
{
    if (dynamic_cast<TitleWorld*>(world))
    {
        constexpr auto base = 9.8f * 300;
        const auto distance = std::clamp(300.0f - transform.translate.y, 0.0f, 100.0f);
        const auto correctedDistance = std::max(distance / 100.0f, 1.0f); // 1px = 1cm, convert cm to meter
        const auto g = base * 0.2f / correctedDistance;
        gravitySources.emplace_back(0.0f, g);
        return;
    }

    for (const auto& other : world->GetComponents())
    {
        if (other == this)
        {
            continue;
        }
        if (const auto& planet = dynamic_cast<PlanetComponent*>(other))
        {
            planet->ApplyGravity(this);
        }
    }
}

void PhysicsComponent::OnCollide(PhysicsComponent* other, const IntersectingResult& result)
{
}
