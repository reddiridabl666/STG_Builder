#pragma once

#include <memory>

#include "AssetManager.hpp"
#include "Displayable.hpp"
#include "Json.hpp"

struct DisplayableFactory {
    static std::unique_ptr<Displayable> create(const nl::json& json, assets::Textures& assets);
};
