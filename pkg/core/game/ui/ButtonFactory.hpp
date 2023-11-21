#pragma once

#include "Button.hpp"
#include "DisplayableFactory.hpp"
#include "Json.hpp"
#include "TextFactory.hpp"

namespace engine {
struct ButtonFactory {
    static Button create(const nl::json& json, assets::Manager& assets) {
        return Button{
            DisplayableFactory::create(json.at("bg"), assets.textures()),
            TextFactory::create(json.at("message"), assets.fonts()),
        };
    }
};
}  // namespace engine
