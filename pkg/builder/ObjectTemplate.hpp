#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "GameObject.hpp"

class ObjectTemplate {
  public:
    using AssetPaths = std::vector<std::string>;

    ObjectTemplate(sf::Vector2f pos, const Asset& image) : pos_(pos), images_{image.path} {}

    ObjectTemplate(sf::Vector2f pos, const AssetPaths& images, const AssetPaths& sounds = AssetPaths{})
        : pos_(pos), images_(images), sounds_(sounds) {}

    const sf::Vector2f& pos() const {
        return pos_;
    }

    const AssetPaths images() const {
        return images_;
    }

    const AssetPaths sounds() const {
        return sounds_;
    }

    virtual GameObject create_object() const = 0;

  private:
    AssetPaths images_;
    AssetPaths sounds_;
    sf::Vector2f pos_;
};
