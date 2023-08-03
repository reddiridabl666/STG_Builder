#pragma once

#include <fmt/core.h>

#include <memory>
#include <string>
#include <tl/expected.hpp>

#include "Errors.hpp"
#include "Loadable.hpp"

struct AssetLoader {
    std::string base_folder = "";

    template <Loadable T>
    ErrorOr<std::shared_ptr<T>> load(const std::string& path) const {
        auto asset = std::make_shared<T>();
        bool res = asset->loadFromFile(base_folder + "/" + path);

        if (!res) {
            return unexpected_error<InternalError>(
                fmt::format("Failure loading asset {}/{}", base_folder, path));
        }
        return asset;
    }
};
