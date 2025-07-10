#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../component/CameraComponent.h"
#include "../component/Component.h"
#include "../component/PhysicsComponent.h"
#include "../component/PlayerComponent.h"
#include "../component/misc/BallisticComponent.h"
#include "../component/misc/MenuComponent.h"
#include "../graph/Graph.h"

enum class WorldType
{
    Splash,
    Title,
    Demo,
    StageSelect,
    Editor,
    Play,
};

class World
{
    int next_id_ = 0;
    bool menu_opened_ = false;
    bool menu_key_pressing_ = false;

    std::vector<std::shared_ptr<Component>> add_queue_ = {};
    std::vector<int> remove_queue_ = {};
    std::map<int, std::shared_ptr<Component>> component_map_ = {};

    std::shared_ptr<CameraComponent> camera_;
    std::shared_ptr<PlayerComponent> player_;
    std::shared_ptr<BallisticComponent> ballistic_;
    std::shared_ptr<MenuComponent> menu_;

public:
    bool zoomEnabled = true;
    bool initialized = false;
    bool menuEnabled = true;

    World();
    virtual ~World() = default;

    virtual void Init();
    virtual void Draw();
    virtual void Update(const float& deltaTime);
    virtual void PostUpdate(const float& deltaTime);

    virtual void DrawBallistic();

    /**
     * ワールド内で一意なIDを生成する
     * @return コンポーネントID
     */
    int NextComponentId();

    /**
     * ワールドにコンポーネントを追加する
     * @param component 
     * @return 追加されたらtrue
     */
    bool AddComponent(const std::shared_ptr<Component>& component);

    /**
     * コンポーネントを削除する
     * @param id コンポーネントID
     * @return 削除されたらtrue
     */
    bool RemoveComponent(int id);

    /**
     * ワールド内のすべてのコンポーネントを取得する
     * @return 
     */
    std::vector<Component*> GetComponents();

    Component* GetComponent(int id);

    [[nodiscard]] CameraComponent& GetCamera() const;

    [[nodiscard]] PlayerComponent* GetPlayer() const;

    [[nodiscard]] std::vector<PhysicsComponent*> GetNearbyPhysicsComponents(const Vec2& origin, float radius) const;

    [[nodiscard]] virtual WorldType GetType() const;

    virtual void OnCameraMoveWithMouse(CameraComponent* camera);

    virtual void OnShot();
    virtual void OnGoal();

    virtual bool CanPlayerShot();

    void SetMenuOpen(bool open) const;

    DrawStack CreateDrawStack() const;

protected:
    virtual void DrawBackground(DrawStack& stack);
    virtual void PostDraw(DrawStack& stack);

    [[nodiscard]] Vec2 GetWorldPos(const Vec2& screenPos) const;
};
