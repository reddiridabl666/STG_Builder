#pragma once

#include "Button.hpp"
#include "DisplayableFactory.hpp"
#include "Json.hpp"
#include "TextFactory.hpp"

namespace engine {
struct ButtonFactory {
    static Button create(const std::string& text, const nl::json& json, assets::Manager& assets) {
        return Button{
            DisplayableFactory::create(json, assets.textures()),
            TextFactory::create(json, assets.fonts(), text),
        };
    }

    static std::unique_ptr<Button> create_unique(const std::string& text, const nl::json& json,
                                                 assets::Manager& assets) {
        return std::make_unique<Button>(DisplayableFactory::create(json, assets.textures()),
                                        TextFactory::create(json, assets.fonts(), text));
    }
};
}  // namespace engine
