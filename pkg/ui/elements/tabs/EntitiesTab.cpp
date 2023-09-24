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
    EntitiesTabContents(AssetManager<sf::Texture>& textures, Box<EntityEntry>::Items&& items, nl::json& data)
        : create_btn_(message_func(Message::CreateObjectType), textures.get_or("plus.png", kFallbackImage),
                      ImVec2{50, 50},
                      [this, &textures] {
                          auto name = fmt::format("New object {}", box_.size());

                          data_[name] = nl::json::object();
                          Bus::get().emit(Bus::Event::ObjectTypesChanged, data_);

                          box_.elems().push_back(std::make_unique<EntityEntry>(
                              name, &data_.at(name), *textures.get(kFallbackImage)));
                      },
                      true, {}, {400, 80}),
          box_(std::move(items), {}),
          data_(data) {}

    void draw(const Window& window) override {
        std::erase_if(box_.elems(), [this](auto& entry) {
            if (entry->should_delete()) {
                data_.erase(entry->get_name());
                Bus::get().emit(Bus::Event::ObjectTypesChanged, data_);
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
        Bus::get().emit(Bus::Event::ObjectTypesChanged, data_);
    }

  private:
    ImageButton create_btn_;
    Box<EntityEntry> box_;
    nl::json& data_;
};
}  // namespace

Menu::Tab EntitiesTab(const std::filesystem::path& game_dir, AssetManager<sf::Texture>& textures,
                      nl::json& json) {
    Box<EntityEntry>::Items entries;
    entries.reserve(json.size());

    for (auto& [key, val] : json.items()) {
        entries.push_back(std::make_unique<EntityEntry>(
            key, &val,
            textures.get_or(game_dir / "assets/images" / json::get<std::string>(val, "image"),
                            kFallbackImage)));
    }

    auto tab = std::make_unique<EntitiesTabContents>(textures, std::move(entries), json);

    return Menu::Tab(std::move(tab), message_func(Message::ObjectTypes));
}
}  // namespace ui
