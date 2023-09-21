#pragma once

#include "DrawableRTree.hpp"
#include "Game.hpp"

namespace builder {
class EditableGame : public engine::Game<DrawableRTree<>> {
  public:
    using Game<DrawableRTree<>>::Game;

    auto& textures() {
        return textures_;
    }

    void scroll(float);
    void zoom(float);

    Error choose_level(size_t);

    void render_debug();
    void reload_objects();

    GameObject* object_by_pos(const sf::Vector2f& pos);

  private:
    static constexpr const char* kOptsID = "__opts_id";

    ErrorOr<GameObject> generate_object_debug(size_t idx, const ObjectOptions& opts);
};
}  // namespace builder
