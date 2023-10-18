#pragma once

#include "DrawableRTree.hpp"
#include "Game.hpp"

namespace builder {
static constexpr const char* kOptsID = "__opts_id";
static constexpr const char* kJsonID = "__json_id";

class EditableGame : public engine::Game<DrawableRTree<>> {
  public:
    static inline sf::Vector2f kInitSize = {100, 100};

    using Game<DrawableRTree<>>::Game;

    auto& textures() {
        return textures_;
    }

    void scroll(float);
    void move_view(const sf::Vector2f& vec);
    void zoom(float);

    GameObject& new_object(const std::string& type);
    void new_object_type(const std::string& type);
    void remove_object(const std::string& name);
    GameObject& reload_object(const std::string& name, const ObjectOptions& opts);

    Error choose_level(size_t);

    void render_debug();
    void reload_objects();
    Error prepare_preview(size_t level);

    void set_object_pos(GameObject& obj, const sf::Vector2f& pos);
    GameObject* get_object(const sf::Vector2f& pos);
    GameObject* get_object(const std::string& name);

    const GameField& field() const {
        return level_->field();
    }

    ~EditableGame();

  private:
    ErrorOr<GameObject> generate_object_debug(size_t idx, const ObjectOptions& opts);
    void scroll_back();

    float scrolled_ = 0;
};
}  // namespace builder
