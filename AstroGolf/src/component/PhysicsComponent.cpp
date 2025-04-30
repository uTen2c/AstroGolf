#include "PhysicsComponent.h"

#include <DxLib.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "../Game.h"
#include "../math/BoundingBox.h"
#include "../math/CircleCollider.h"
#include "../math/NullCollider.h"
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

PhysicsComponent::PhysicsComponent(const int id): Component(id)
{
    collider = std::make_unique<NullCollider>();
}

void PhysicsComponent::Update(const float deltaTime)
{
    Component::Update(deltaTime);

    // if (velocity.Length() > 0)
    // {
    auto copied = velocity;
    // copied.Normalize();
    // copied.Mul(min(velocity.Length(), 500.0f));
    copied.Mul(deltaTime);

    copied.Add(gravityVelocity.Copy().Mul(deltaTime));

    // if (gravity > 0 && !intersecting_)
    // {
        // velocity.y += gravity * Game::deltaTime;
    // }

    Move(copied);
    // }
}

void PhysicsComponent::Draw(DrawStack* stack)
{
    Component::Draw(stack);
}

void PhysicsComponent::Move(const Vec2& delta)
{
    const auto nearbyComponents = world->GetNearbyPhysicsComponents(transform.translate, 1000);
    auto moved = Vec2(transform.translate.x, transform.translate.y);
    Vec2 normal;

    if (const auto selfCc = dynamic_cast<CircleCollider*>(collider.get()))
    {
        moved.Add(delta);

        for (const auto& nearbyComponent : nearbyComponents)
        {
            if (nearbyComponent == this)
            {
                continue;
            }

            if (const auto cc = dynamic_cast<CircleCollider*>(nearbyComponent->collider.get()))
            {
                const auto otherPos = nearbyComponent->transform.translate;
                const auto distance = otherPos.Distance(moved);

                if (distance >= selfCc->radius + cc->radius)
                {
                    continue;
                }

                auto negDelta = delta;
                negDelta.Mul(-1);
                negDelta.Mul(10);
                velocity.Add(negDelta);

                auto negVec = moved;
                negVec.Sub(otherPos);
                negVec.Normalize();

                const auto diff = selfCc->radius + cc->radius - distance;
                negVec.Mul(diff);

                moved.Add(negVec);
            }
            if (const auto bb = dynamic_cast<BoundingBox*>(nearbyComponent->collider.get()))
            {
                const auto otherPos = nearbyComponent->transform.translate;
                const auto x1 = otherPos.x + bb->GetLeft();
                const auto x2 = otherPos.x + bb->GetRight();
                const auto y1 = otherPos.y + bb->GetTop();
                const auto y2 = otherPos.y + bb->GetBottom();
                const auto r = selfCc->radius;

                // Top floor
                if (moved.x > x1 && moved.x < x2 && moved.y + r >= y1 && moved.y < y2)
                {
                    moved.y = y1 - r;
                    normal = {0, 1};
                    continue;
                }
                // Bottom floor
                if (moved.x > x1 && moved.x < x2 && moved.y > y1 && moved.y - r <= y2)
                {
                    moved.y = y2 + r;
                    normal = {0, -1};
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

                const auto diff = selfCc->radius - hitCorner.Distance(moved);
                negVec.Mul(diff);

                moved.Add(negVec);

                auto negDelta = delta;
                negDelta.Mul(-1);
                negDelta.Mul(10);
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

                const auto& otherCollider = nearbyComponent->collider;
                if (const BoundingBox* bb = dynamic_cast<BoundingBox*>(otherCollider.get()))
                {
                    if (collider->Intersects(moved, nearbyComponent->transform.translate, *bb))
                    {
                        if (delta.x > 0)
                        {
                            const auto otherEdgeX = nearbyComponent->transform.translate.x - bb->width / 2;
                            moved.x = otherEdgeX - GetColliderRadiusX(collider.get());
                        }
                        else
                        {
                            const auto otherEdgeX = nearbyComponent->transform.translate.x + bb->width / 2;
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

                const auto& otherCollider = nearbyComponent->collider;
                if (const BoundingBox* bb = dynamic_cast<BoundingBox*>(otherCollider.get()))
                {
                    if (collider->Intersects(moved, nearbyComponent->transform.translate, *bb))
                    {
                        if (delta.y > 0)
                        {
                            const auto otherEdgeY = nearbyComponent->transform.translate.y - bb->height / 2;
                            moved.y = otherEdgeY - GetColliderRadiusY(collider.get());
                        }
                        else
                        {
                            const auto otherEdgeY = nearbyComponent->transform.translate.y + bb->height / 2;
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

    ImGui::Begin("intersecting_");
    ImGui::Text(intersecting_ ? "TRUE" : "FALSE");
    ImGui::End();

    // if (normal.Length() > 0)
    // {
    //     float separatingVelocity = velocity.Dot(normal);
    //     if (separatingVelocity < 0)
    //     {
    //         // 反発係数を考慮した反発後の速度成分を計算
    //         // e = -(v_after) / (v_before) => v_after = -e * v_before
    //         //  impulse = normal * mass * (-(1 + e) * separatingVelocity) / (mass) // 相手が静的な場合
    //         Vec2 impulse = normal;
    //         auto restitution = 0.2f;
    //         impulse.Mul(-(1.0f + restitution) * separatingVelocity);
    //
    //         // 地面との衝突の場合、水平方向の速度への影響を減らす (オプション)
    //         if (intersecting_ && std::abs(normal.y) > 0.7f) // 地面法線に近い場合
    //         {
    //             // 垂直方向のインパルスのみ適用する、など
    //             velocity.y += impulse.y;
    //             // 水平方向は摩擦で処理するため、ここでは変更しないか、影響を小さくする
    //             // velocity.x += impulse.x * 0.1f; // 例: 水平インパルスの影響を減らす
    //         }
    //         else // 壁などとの衝突
    //         {
    //             velocity.Add(impulse); // 計算されたインパルスを速度に加える
    //         }
    //
    //         // 閾値以下の速度なら停止（微小な振動を防ぐ）
    //         // 特に地面での静止状態を実現するために重要
    //         if (intersecting_ && velocity.Length() < 0.1f
    //         )
    //         {
    //             velocity = {0, 0};
    //         }
    //         // 地面で、かつ垂直方向の速度が非常に小さい場合、垂直速度を0にする
    //         if (intersecting_ && std::abs(velocity.y) < 0.5f)
    //         {
    //             velocity.y = 0;
    //         }
    //     }
    // }

    spdlog::info("{}", velocity.y);
    transform.translate = moved;
}
