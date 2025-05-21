#include "TitleWorld.h"

#include "../component/RotatableBoxComponent.h"
#include "../math/Math.h"

TitleWorld::TitleWorld()
{
    // zoomEnabled = false;

    const auto ground = std::make_shared<RotatableBoxComponent>(NextComponentId(), 1000, 200);
    ground->transform.translate = {0, 300};
    AddComponent(ground);
}

void TitleWorld::Update(const float& deltaTime)
{
    constexpr auto base = 9.8f * 200;
    const auto playerDistance = 200.0f - GetPlayer()->transform.translate.y;
    const auto distance = std::max(playerDistance / 100.0f, 1.0f); // 1px = 1cm, convert cm to meter
    const auto g = base * 0.2f / distance;
    GetPlayer()->gravitySources.emplace_back(0.0f, g);

    World::Update(deltaTime);
}

WorldType TitleWorld::GetType() const
{
    return WorldType::Title;
}
