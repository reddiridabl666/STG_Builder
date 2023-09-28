#pragma once

#include <variant>

#include "AssetManager.hpp"
#include "Box.hpp"
#include "Button.hpp"
#include "Json.hpp"
#include "Menu.hpp"
#include "Messages.hpp"
#include "StringLiteral.hpp"

namespace ui {

template <typename T>
concept EntryType = requires(T a) {
    requires std::derived_from<T, Element>;
    { a.get_name() } -> std::convertible_to<std::variant<std::string, size_t>>;
    { a.to_json() } -> std::convertible_to<nl::json>;
    { a.should_delete() } -> std::convertible_to<bool>;
};

template <EntryType T, Message btn_name_, StringLiteral create_template_>
struct EntriesTab : public Element {
  public:
    EntriesTab(AssetManager<sf::Texture>& textures, Box<T>::Items&& items, nl::json& data)
        : create_btn_(message_func(btn_name_), textures.get_or("plus.png", kFallbackImage), ImVec2{50, 50},
                      [this, &textures] {
                          auto name = fmt::format(create_template_.value, box_.size());
                          box_.elems().push_back(std::make_unique<T>(name, *textures.get(kFallbackImage)));
                      },
                      true, {}, {400, 80}),
          box_(std::move(items), {}),
          data_(data) {}

    void draw(const Window& window) override {
        std::erase_if(box_.elems(), [](auto& entry) {
            return entry->should_delete();
        });

        create_btn_.draw(window);
        box_.draw(window);
    }

    ~EntriesTab() {
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
    }

  private:
    ImageButton create_btn_;
    Box<T> box_;
    nl::json& data_;
};
}  // namespace ui
