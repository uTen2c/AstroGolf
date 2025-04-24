#pragma once
#include <map>
#include <memory>
#include <vector>

#include "../camera/GameCamera.h"
#include "../component/Component.h"

class World
{
    int next_id_ = 0;
    std::map<int, std::shared_ptr<Component>> component_map_ = {};

    std::unique_ptr<GameCamera> camera_ = std::make_unique<GameCamera>();

public:
    World();
    ~World() = default;

    void Draw();

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
    std::vector<const Component*> GetComponents();

    const Component* GetComponent(int id);

    [[nodiscard]] GameCamera& GetCamera() const;
};
