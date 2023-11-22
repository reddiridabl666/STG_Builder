#pragma once

#include "AssetManager.hpp"
#include "Json.hpp"
#include "Text.hpp"

namespace engine {
struct TextFactory {
    static Text create(const TextProps& props, assets::Fonts& fonts) {
        return Text{
            props.text,
            fonts.get_or(props.font, assets::kFallbackFont),
            props.font_size,
            props.color,
        };
    }

    static Text create(const nl::json& json, assets::Fonts& fonts, const std::string& fallback_text = "") {
        auto props = json.get<TextProps>();
        if (props.text == "") {
            props.text = fallback_text;
        }

        return create(props, fonts);
    }
};
}  // namespace engine
