#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "AssetLoader.hpp"
#include "AssetStorage.hpp"

namespace assets {
static constexpr const char* kFallbackImage = "fallback.png";

template <Loadable T>
class Manager {
  public:
    Manager(const std::string& dir = "") : loader_{dir}, storage_() {}

    Manager(Loader&& loader, Storage<T>&& storage)
        : loader_(std::move(loader)), storage_(std::move(storage)) {}

    Loader& loader() {
        return loader_;
    }

    Storage<T>& storage() {
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
    Loader loader_;
    Storage<T> storage_;
};

using Textures = Manager<sf::Texture>;
using Sounds = Manager<sf::SoundBuffer>;
}  // namespace assets
