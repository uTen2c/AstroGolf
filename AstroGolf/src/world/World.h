#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../component/CameraComponent.h"
#include "../component/Component.h"
#include "../component/PhysicsComponent.h"
#include "../component/PlayerComponent.h"
#include "../component/misc/BallisticComponent.h"
#include "../graph/Graph.h"

enum class WorldType
{
    Title,
    Demo,
    StageSelect,
};

class World
{
    int next_id_ = 0;
    bool menu_opened_ = false;
    float menu_transition_delta_ = 0;
    bool menu_key_pressing_ = false;

    std::map<int, std::shared_ptr<Component>> component_map_ = {};

    std::shared_ptr<CameraComponent> camera_;
    std::shared_ptr<PlayerComponent> player_;
    std::shared_ptr<BallisticComponent> ballistic_;

    std::unique_ptr<Graph> menu_button_graph_;
    std::unique_ptr<Graph> menu_background_graph_;
    std::unique_ptr<Graph> menu_buttons_graph_;

public:
    bool zoomEnabled = true;
    bool initialized = false;

    World();
    virtual ~World() = default;

    virtual void Init();
    virtual void Draw();
    virtual void Update(const float& deltaTime);
    virtual void PostUpdate(const float& deltaTime);

    virtual void DrawUi();
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

    virtual void OnGoal();

    virtual bool CanPlayerShot();

    void SetMenuOpen(bool open);

protected:
    virtual void DrawBackground(DrawStack& stack) const;
};
