#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "Text.hpp"

namespace engine {
struct TextFactory {
    static Text create(const nl::json& json, assets::Fonts& fonts) {
        return Text{
            json.value("text", ""),
            fonts.get_or(json.value("font", ""), assets::kFallbackFont),
            json.value("size", 30ul),
        };
    }
};
}  // namespace engine
