#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "AssetLoader.hpp"
#include "AssetStorage.hpp"

static constexpr const char* kFallbackImage = "fallback.png";

template <Loadable T>
class AssetManager {
  public:
    AssetManager(const std::string& dir) : loader_{dir}, storage_() {}

    AssetManager(AssetLoader&& loader, AssetStorage<T>&& storage)
        : loader_(std::move(loader)), storage_(std::move(storage)) {}

    AssetLoader& loader() {
        return loader_;
    }

    AssetStorage<T>& storage() {
        return storage_;
    }

    ErrorOr<std::shared_ptr<T>> get(const std::string& filename) {
        if (auto asset = storage_.get(filename)) {
            return asset;
        }

        auto asset = loader_.load<T>(filename);
        if (asset) {
            storage_.add(filename, asset.value());
        }

        return asset;
    }

    std::shared_ptr<T> get_or(const std::string& filename, const std::string& fallback) {
        auto res = get(filename)
                       .or_else([this, &fallback](auto) {
                           return get(fallback);
                       })
                       .or_else([&fallback](auto) {
                           throw std::runtime_error("Missing fallback asset: " + fallback);
                       });

        return *res;
    }

  private:
    AssetLoader loader_;
    AssetStorage<T> storage_;
};
