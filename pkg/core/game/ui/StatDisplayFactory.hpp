#pragma once

#include <memory>

#include "AssetManager.hpp"
#include "Bar.hpp"
#include "Counter.hpp"
#include "Json.hpp"
#include "MaxCounter.hpp"

class StatDisplayFactory {
  public:
    static std::unique_ptr<GameUi> create(float init_val, const nl::json&, assets::Manager&);
};

// clang-format off
std::unique_ptr<GameUi> StatDisplayFactory::create(float init_val, const nl::json& json, assets::Manager& assets) {
    auto type = json::get<std::string>(json, "type");

    if (type == "bar") {
        auto empty = assets.textures().get_or(json::get<std::string>(json, "empty"), assets::kFallbackImage);
        return std::make_unique<Bar>(
            init_val,
            json::get<float>(json, "width", empty->getSize().x),
            std::move(empty),
            assets.textures().get_or(json::get<std::string>(json, "full"), assets::kFallbackImage)
        );
    }
    
    auto font = json::get<std::string>(json, "font");

    if (type == "counter") {
        return std::make_unique<Counter>(
            init_val,
            json::get<std::string>(json, "prefix"),
            assets.fonts().get_or(font, assets::kFallbackFont),
            json::get<size_t>(json, "size", 30)
        );
    }

    if (type == "max_counter") {
        return std::make_unique<MaxCounter>(
            init_val,
            json::get<std::string>(json, "prefix"),
            assets.fonts().get_or(font, assets::kFallbackFont),
            json::get<size_t>(json, "size", 30)
        );
    }

    return nullptr;
}
// clang-format on
