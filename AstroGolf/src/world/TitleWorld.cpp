#include "TitleWorld.h"

#include "../component/RotatableBoxComponent.h"
#include "../math/Math.h"

TitleWorld::TitleWorld()
{
    zoomEnabled = false;
    
    const auto ground = std::make_shared<RotatableBoxComponent>(NextComponentId(), 1000, 100);
    ground->transform.translate = {0, 300};
    AddComponent(ground);
}

void TitleWorld::Update(const float& deltaTime)
{
    World::Update(deltaTime);

    GetPlayer()->gravitySources.emplace_back(0.0f, 9.8f * 50);
}

WorldType TitleWorld::GetType() const
{
    return WorldType::Title;
}
