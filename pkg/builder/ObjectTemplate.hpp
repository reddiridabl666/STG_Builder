#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "GameObject.hpp"

class ObjectTemplate {
  public:
    using AssetPaths = std::unordered_map<AssetType, std::vector<std::string>>;

    ObjectTemplate(sf::Vector2f pos, const Asset& asset)
        : pos_(pos), paths_{{asset.type, std::vector<std::string>{asset.path}}} {}

    ObjectTemplate(sf::Vector2f pos, const AssetPaths& paths) : pos_(pos), paths_(paths) {}

    const sf::Vector2f& pos() const {
        return pos_;
    }

    const std::vector<std::string>& asset_paths(AssetType type) {
        return paths_[type];
    }

    virtual GameObject create_object() const = 0;

  private:
    AssetPaths paths_;
    sf::Vector2f pos_;
};
