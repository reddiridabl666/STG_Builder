#pragma once

#include <memory>

#include "AssetManager.hpp"
#include "Bar.hpp"
#include "Counter.hpp"
#include "Json.hpp"
#include "MaxCounter.hpp"

class StatDisplayFactory {
    std::unique_ptr<GameUi> create(float init_val, const nl::json&, assets::Manager&);
};

// clang-format off
std::unique_ptr<GameUi> StatDisplayFactory::create(float init_val, const nl::json& json, assets::Manager& assets) {
    auto type = json::get<std::string>(json, "type");

    if (type == "bar") {
        return std::make_unique<Bar>(
            init_val,
            assets.textures().get_or(json.at("empty").get<std::string>(), assets::kFallbackImage),
            assets.textures().get_or(json.at("full").get<std::string>(), assets::kFallbackImage)
        );
    }

    if (type == "counter") {
        return std::make_unique<Counter>(
            init_val,
            assets.fonts().get_or(json.at("font").get<std::string>(), assets::kFallbackFont),
            json::get<size_t>(json, "size", 30)
        );
    }

    if (type == "max_counter") {
        return std::make_unique<MaxCounter>(
            init_val,
            assets.fonts().get_or(json.at("font").get<std::string>(), assets::kFallbackFont),
            json::get<size_t>(json, "size", 30)
        );
    }

    return nullptr;
}
// clang-format on
