#include "EntitiesTab.hpp"

#include "Box.hpp"
#include "GameInfo.hpp"
#include "ImguiUtils.hpp"
#include "Messages.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

#include "Bus.hpp"
#include "EntityEntry.hpp"

namespace ui {
namespace {
struct EntitiesTabContents : public Element {
  public:
    EntitiesTabContents(assets::Textures& textures, Box<EntityEntry>::Items&& items, nl::json& data)
        : create_btn_(message_func(Message::CreateObjectType),
                      textures.get_or("plus.png", assets::kFallbackImage), ImVec2{50, 50},
                      [this, &textures] {
                          auto name = fmt::format("New object {}", box_.size());

                          data_[name] = {{"size", sf::Vector2f{100, 100}}, {"speed", 10}};
                          Bus<nl::json>::get().emit(Event::ObjectTypesChanged, data_);
                          Bus<std::string>::get().emit(Event::ObjectTypeCreated, name);

                          box_.elems().push_back(std::make_unique<EntityEntry>(
                              name, &data_.at(name), *textures.get(assets::kFallbackImage), 0));
                      },
                      true, {}, {400, 80}),
          box_(std::move(items), {}),
          data_(data) {
        Bus<std::string>::get().on(
            Event::ObjectDeleted, "entities_tab_obj_deleted", [this](const nl::json& type) {
                auto it = std::ranges::find_if(box_.elems(), [&type](const auto& elem) {
                    return elem->get_name() == type.template get<std::string>();
                });

                if (it != box_.elems().end()) {
                    (*it)->obj_count() -= 1;
                }
            });
    }

    void draw(const Window& window) override {
        std::erase_if(box_.elems(), [this](auto& entry) {
            if (entry->should_delete()) {
                data_.erase(entry->get_name());
                Bus<nl::json>::get().emit(Event::ObjectTypesChanged, data_);
                return true;
            }
            return false;
        });

        create_btn_.draw(window);
        box_.draw(window);
    }

    ~EntitiesTabContents() {
        auto backup = data_;
        data_.clear();
        try {
            for (const auto& entry : box_.elems()) {
                data_[entry->get_name()] = entry->to_json();
            }
        } catch (std::exception& e) {
            data_ = backup;
#ifdef DEBUG
            LOG(e.what());
#endif
        }
        Bus<nl::json>::get().emit(Event::ObjectTypesChanged, data_);
        Bus<std::string>::get().off(Event::ObjectDeleted, "entities_tab_obj_deleted");
    }

  private:
    ImageButton create_btn_;
    Box<EntityEntry> box_;
    nl::json& data_;
};
}  // namespace

Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, assets::Textures& textures,
                      nl::json& entities_json, const nl::json& level_objects) {
    Box<EntityEntry>::Items entries;
    entries.reserve(entities_json.size());

    for (auto& [key, val] : entities_json.items()) {
        entries.push_back(std::make_unique<EntityEntry>(
            key, &val,
            textures.get_or(game_dir / "assets/images" / json::get<std::string>(val, "image"),
                            assets::kFallbackImage),
            std::ranges::count_if(level_objects, [&key](const auto& elem) {
                return elem.at("type") == key;
            })));
    }

    auto tab = std::make_unique<EntitiesTabContents>(textures, std::move(entries), entities_json);

    return Menu::Tab(std::move(tab), message_func(Message::ObjectTypes));
}
}  // namespace ui
