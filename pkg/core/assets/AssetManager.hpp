#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "AssetLoader.hpp"
#include "AssetStorage.hpp"

namespace assets {
static inline const std::string kFallbackImage = "fallback.png";
static inline const std::string kFallbackFont = "Roboto-Regular.ttf";

template <Loadable T>
class TypedManager {
  public:
    TypedManager(const std::string& dir = "") : loader_{dir}, storage_() {}

    TypedManager(Loader&& loader, Storage<T>&& storage) : loader_(std::move(loader)), storage_(std::move(storage)) {}

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

using Textures = TypedManager<sf::Texture>;
using Sounds = TypedManager<sf::SoundBuffer>;
using Fonts = TypedManager<sf::Font>;

class Manager {
  public:
    Manager(const std::string& textures, const std::string& sounds, const std::string& fonts)
        : textures_(textures), sounds_(sounds), fonts_(fonts) {}

    auto& textures() {
        return textures_;
    }

    auto& sounds() {
        return sounds_;
    }

    auto& fonts() {
        return fonts_;
    }

    void clear_unused() {
        textures_.storage().clear_unused();
        sounds_.storage().clear_unused();
        fonts_.storage().clear_unused();
    }

  private:
    Textures textures_;
    Sounds sounds_;
    Fonts fonts_;
};
}  // namespace assets
