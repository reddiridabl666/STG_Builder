#pragma once

#include <concepts>
// #include <expected>
#include <string>

#include "Loadable.hpp"

struct AssetLoader {
    std::string base_folder;

    // template <Loadable T>
    // std::expected<T, bool> load(const std::string& path) const {
    //     T asset{};
    //     auto res = asset.loadFromFile(base_folder + "/" + path);

    //     if (!res) {
    //         return std::unexpected(res);
    //     }
    //     return asset;
    // }

    template <Loadable T>
    std::pair<T, bool> load(const std::string& path) const {
        T asset{};
        auto res = asset.loadFromFile(base_folder + "/" + path);
        return std::make_pair(asset, res);
    }
};
