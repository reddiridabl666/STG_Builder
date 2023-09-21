#pragma once

#include "ImguiUtils.hpp"
#include "Json.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include <unordered_set>

#include "UiElement.hpp"

namespace ui {
struct EntityEntry : public Element {
  public:
    EntityEntry() = default;

    EntityEntry(const std::string& name, std::shared_ptr<sf::Texture>&& image)
        : texture_(std::move(image)), name(name), old_name_(name) {}

    EntityEntry(const std::string& name, nl::json& json, std::shared_ptr<sf::Texture>&& image);

    nl::json to_json() const;

    const std::string& get_name() const {
        return name;
    }

    void draw(const Window&) override;

    bool should_delete() const {
        return to_delete_;
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(EntityEntry, image, speed, description, tag, size)

  private:
    std::shared_ptr<sf::Texture> texture_;

    static const std::unordered_set<std::string> kBaseValues;

    std::string name;
    std::string image = "";
    std::string description = "Description goes here";
    std::string tag = "";
    sf::Vector2f size = {};
    float speed = 0;

    std::unordered_map<std::string, float> values_;
    bool shown_ = false;
    std::string stat_name_;
    std::string old_name_;
    bool to_delete_ = false;
};
}  // namespace ui
