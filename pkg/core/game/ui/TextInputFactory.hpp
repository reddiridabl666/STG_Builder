#pragma once

#include "AssetManager.hpp"
#include "DisplayableFactory.hpp"
#include "Json.hpp"
#include "TextFactory.hpp"
#include "TextInput.hpp"

namespace engine {

struct TextInputFactory {
    static std::unique_ptr<TextInput> create(Window& window, const nl::json& json, assets::Manager& assets) {
        return std::make_unique<TextInput>(
            window, DisplayableFactory::create(json.value("bg", nl::json::object()), assets.textures()),
            TextFactory::create(json.value("text", nl::json::object()), assets.fonts()));
    }
};
}  // namespace engine
