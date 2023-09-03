#pragma once

#include <fmt/core.h>

#include <memory>
#include <string>
#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Loadable.hpp"

#ifdef DEBUG
#include "Debug.hpp"
#endif

struct AssetLoader {
    std::string base_folder = "";

    template <Loadable T>
    ErrorOr<std::shared_ptr<T>> load(const std::string& path) const {
#ifdef DEBUG
        LOG(fmt::format("Loading asset: {}", path));
#endif
        auto asset = std::make_shared<T>();
        bool res = asset->loadFromFile(base_folder + "/" + path);

        if (!res) {
            return Error::New(fmt::format("Failure loading asset {}/{}", base_folder, path));
        }
        return asset;
    }
};
