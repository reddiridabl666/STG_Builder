#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "AssetLoader.hpp"
#include "AssetStorage.hpp"

template <Loadable T>
class AssetManager {
  public:
    AssetManager(AssetLoader&& loader, AssetStorage<T>&& storage)
        : loader_(std::move(loader)), storage_(std::move(storage)) {}

    AssetLoader& loader() {
        return loader_;
    }

    AssetStorage<T>& storage() {
        return storage_;
    }

    auto get(const std::string& filename) {
        if (auto asset = storage_.get(filename)) {
            return asset;
        }

        auto asset = loader_.load<T>(filename);
        if (asset) {
            storage_.add(filename, asset.value());
        }

        return asset;
    }

  private:
    AssetLoader loader_;
    AssetStorage<T> storage_;
};
