#pragma once

#include <memory>

#include "AssetManager.hpp"
#include "Bar.hpp"
#include "Counter.hpp"
#include "Json.hpp"
#include "MaxCounter.hpp"
#include "Value.hpp"

class StatDisplayFactory {
  public:
    static std::unique_ptr<GameUi> create(Value value, const nl::json&, assets::Manager&);
};

// clang-format off
std::unique_ptr<GameUi> StatDisplayFactory::create(Value value, const nl::json& json, assets::Manager& assets) {
    auto type = json.value("type", "");

    if (type == "bar") {
        auto empty = assets.textures().get_or(json.value("empty", ""), assets::kFallbackImage);
        return std::make_unique<Bar>(
            value.get(),
            value.default_value(),
            json.value("width", empty->getSize().x),
            std::move(empty),
            assets.textures().get_or(json.value("full", ""), assets::kFallbackImage)
        );
    }
    
    auto font = json.value("font", "");

    if (type == "counter") {
        return std::make_unique<Counter>(
            value.get(),
            json.value("prefix", ""),
            assets.fonts().get_or(font, assets::kFallbackFont),
            json.value("size", 30)
        );
    }

    if (type == "max_counter") {
        return std::make_unique<MaxCounter>(
            value.get(),
            value.default_value(),
            json.value("prefix", ""),
            assets.fonts().get_or(font, assets::kFallbackFont),
            json.value("size", 30)
        );
    }

    return nullptr;
}
// clang-format on
