#include "World.h"

#include <DxLib.h>
#include <ranges>
#include <spdlog/spdlog.h>

World::World()
{
    spdlog::info("World init");
    
    camera_ = std::make_shared<CameraComponent>(NextComponentId());

    player_ = std::make_shared<PlayerComponent>(NextComponentId());

    AddComponent(camera_);
    AddComponent(player_);
}

void World::Draw()
{
    auto stack = DrawStack();

    // カメラの位置を補正する
    auto originOffset = Vec2(640, 360); // FIXME マジックナンバーをやめる
    originOffset.Div(camera_->zoom);

    auto trans = camera_->transform.translate;
    trans.Mul(-1);
    trans.Add(originOffset);
    stack.Translate(trans);
    stack.Scale(camera_->zoom);

    DrawBackground(stack);

    stack.Push();
    const auto& pos = stack.GetScreenPos();
    DrawLine(-2000, pos.y, 2000, pos.y, GetColor(255, 0, 0));
    DrawLine(pos.x, -2000, pos.x, 2000, GetColor(0, 255, 0));
    stack.Pop();

    for (const auto& component : GetComponents())
    {
        component->Draw(&stack);
    }
}

void World::Update(const float& deltaTime)
{
    for (const auto component : GetComponents())
    {
        component->Update(deltaTime);
    }
    for (const auto component : GetComponents())
    {
        if (const auto physComp = dynamic_cast<PhysicsComponent*>(component))
        {
            physComp->UpdateMovement(deltaTime);
        }
    }
    for (const auto component : GetComponents())
    {
        component->PostUpdate(deltaTime);
    }
}

int World::NextComponentId()
{
    spdlog::info("{}", next_id_);
    return next_id_++;
}

bool World::AddComponent(const std::shared_ptr<Component>& component)
{
    if (component_map_.contains(component->GetId()))
    {
        spdlog::warn("Failed to add component. ID {} already exists", component->GetId());
        return false;
    }
    component->world = this;
    component_map_.insert(std::make_pair(component->GetId(), component));
    return true;
}

bool World::RemoveComponent(const int id)
{
    if (!component_map_.contains(id))
    {
        return false;
    }

    component_map_.erase(id);
    return true;
}

std::vector<Component*> World::GetComponents()
{
    std::vector<Component*> components;
    components.reserve(component_map_.size());
    std::ranges::transform(component_map_, std::back_inserter(components), [](auto& pair)
    {
        return pair.second.get();
    });
    return components;
}

Component* World::GetComponent(const int id)
{
    const auto found = component_map_.find(id);
    if (found == component_map_.end())
    {
        return nullptr;
    }
    return found->second.get();
}

CameraComponent& World::GetCamera() const
{
    return *camera_;
}

PlayerComponent* World::GetPlayer() const
{
    return player_.get();
}

std::vector<PhysicsComponent*> World::GetNearbyPhysicsComponents(const Vec2& origin, const float radius) const
{
    std::vector<PhysicsComponent*> nearbyComponents;

    if (radius <= 0)
    {
        return nearbyComponents;
    }

    for (const auto& component : component_map_ | std::views::values)
    {
        const auto componentPosition = component->transform.translate;
        const auto distance = componentPosition.Distance(origin);
        const auto physCompPtr = dynamic_cast<PhysicsComponent*>(component.get());
        if (distance <= radius && physCompPtr)
        {
            nearbyComponents.push_back(physCompPtr);
        }
    }

    return nearbyComponents;
}

WorldType World::GetType() const
{
    throw std::runtime_error("Not implemented");
}

void World::DrawBackground(DrawStack& stack) const
{
}
