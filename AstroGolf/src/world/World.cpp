#include "World.h"

#include <spdlog/spdlog.h>

World::World()
{
    spdlog::info("World init");
}

void World::Draw()
{
    auto stack = DrawStack();

    // カメラの位置を補正する
    stack.Translate(camera_->GetPos().GetPoint().Multiply(-1));

    for (const auto& component : GetComponents())
    {
        component->Draw(&stack);
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

std::vector<const Component*> World::GetComponents()
{
    std::vector<const Component*> components;
    components.reserve(component_map_.size());
    std::ranges::transform(component_map_, std::back_inserter(components), [](const auto& pair)
    {
        return pair.second.get();
    });
    return components;
}

const Component* World::GetComponent(const int id)
{
    const auto found = component_map_.find(id);
    if (found == component_map_.end())
    {
        return nullptr;
    }
    return found->second.get();
}

GameCamera& World::GetCamera() const
{
    return *camera_;
}
