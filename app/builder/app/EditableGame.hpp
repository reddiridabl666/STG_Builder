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

    auto& assets() {
        return assets_;
    }

    void scroll(float);
    void move_view(const sf::Vector2f& vec);
    void zoom(float);

    GameObject& new_object(const std::string& type);
    void new_player(const std::string& type);
    void new_object_type(const std::string& type);
    void remove_object(const std::string& name);
    GameObject& reload_object(const std::string& name, ObjectOptions&& opts);

    void update_object_type(const std::string& name, ObjectType&& obj_type);

    Error choose_level(size_t);

    void render_debug();
    void reload_objects();
    void prepare_preview(size_t level);

    void set_object_pos(const std::shared_ptr<GameObject>& obj, const sf::Vector2f& pos);
    std::shared_ptr<GameObject> get_object(const sf::Vector2f& pos);
    std::shared_ptr<GameObject> get_object(const std::string& name);

    void set_game_bg(const std::string& bg_path);
    void set_level_bg(const std::string& bg_path);

    void update_side_menu(const engine::SideMenuProps& props);
    void update_menu_item(size_t id, const nl::json& item);
    void erase_menu_item(size_t id);
    void add_menu_item(const nl::json&);

    const GameField& field() const {
        return level_->field();
    }

    ~EditableGame();

  private:
    std::shared_ptr<GameObject> generate_object_debug(size_t idx, const ObjectOptions& opts);
    void scroll_back();

    float scrolled_ = 0;
};
}  // namespace builder
