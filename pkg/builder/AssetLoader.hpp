#pragma once

#include <concepts>
#include <expected>
#include <string>

template <typename T>
concept Loadable = requires(T a) {
    { a.loadFromFile(std::string{}) } -> std::convertible_to<bool>;
};

struct AssetLoader {
    std::string base_folder;

    template <Loadable T>
    std::expected<T, bool> load(const std::string& path) const {
        T asset{};
        auto res = asset.loadFromFile(base_folder + "/" + path);

        if !res {
            return std::unexpected(res);
        }
        return asset;
    }
};
