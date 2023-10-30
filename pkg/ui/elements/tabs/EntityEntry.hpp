#pragma once

#include "ImguiUtils.hpp"
#include "Json.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include <unordered_set>

#include "UiElement.hpp"
#include "ui/elements/Stats.hpp"

namespace ui {
struct EntityEntry : public Element {
  public:
    EntityEntry() : stats_(kBaseValues) {}
    EntityEntry(const std::string& name, nl::json* json, std::shared_ptr<sf::Texture>&& image, size_t obj_count);

    nl::json to_json() const;

    const std::string& get_name() const {
        return name;
    }

    void draw(const Window&) override;

    bool should_delete() const {
        return to_delete_;
    }

    size_t& obj_count() {
        return obj_count_;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(EntityEntry, image, speed, description, tag, size)

  private:
    std::shared_ptr<sf::Texture> texture_;

    static const std::unordered_set<std::string> kBaseValues;

    std::string name;
    std::string image;
    std::string description = "Description goes here";
    std::string tag = "";
    int tag_id = 0;

    sf::Vector2f size = {100, 100};
    float speed = 0;

    bool shown_ = false;
    std::string old_name_;
    bool to_delete_ = false;

    Stats stats_;

    nl::json* data_;
    size_t obj_count_ = 0;
    bool pop_up_ = false;
};
}  // namespace ui
