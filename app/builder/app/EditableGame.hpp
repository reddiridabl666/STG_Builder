#pragma once

#include "DrawableRTree.hpp"
#include "Game.hpp"

namespace builder {
static constexpr const char* kOptsID = "__opts_id";
static constexpr const char* kJsonID = "__json_id";

class EditableGame : public engine::Game<DrawableRTree<>> {
  public:
    using Game<DrawableRTree<>>::Game;

    auto& textures() {
        return textures_;
    }

    void scroll(float);
    void move_view(const sf::Vector2f& vec);
    void zoom(float);

    GameObject& new_object(const std::string& type);

    Error choose_level(size_t);

    void render_debug();
    void reload_objects();

    GameObject* object_by_pos(const sf::Vector2f& pos);

  private:
    ErrorOr<GameObject> generate_object_debug(size_t idx, const ObjectOptions& opts);
};
}  // namespace builder
