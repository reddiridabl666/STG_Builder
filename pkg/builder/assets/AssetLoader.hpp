#pragma once

#include <concepts>
#include <expected>
#include <string>

#include "Error.hpp"
#include "Loadable.hpp"

struct AssetLoader {
    std::string base_folder;

    template <Loadable T>
    std::expected<T, Error> load(const std::string& path) const {
        T asset{};
        auto res = asset.loadFromFile(base_folder + "/" + path);

        if (!res) {
            return std::unexpected<Error>(Error::Code::Internal);
        }
        return asset;
    }
};
