#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "Text.hpp"

namespace engine {
struct TextFactory {
    static Text create(const nl::json& json, assets::Fonts& fonts, const std::string& fallback_text = "") {
        return Text{
            json.value("text", fallback_text),
            fonts.get_or(json.value("font", ""), assets::kFallbackFont),
            json.value("font_size", 30ul),
        };
    }
};
}  // namespace engine
