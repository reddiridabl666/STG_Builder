#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "Properties.hpp"

class ObjectTemplate {
  public:
    using AssetPaths = std::vector<std::string>;

    // ObjectTemplate(const std::string& name, const Properties::Data& props)
    //     : Properties(props), name_(name) {}

    ObjectTemplate(const std::string& name, int speed, const Properties& props, const Asset& image)
        : name_(name), speed_(speed), props_(props), images_{image.path} {}

    ObjectTemplate(const std::string& name, int speed, const Properties& props, const AssetPaths& images,
                   const AssetPaths& sounds = AssetPaths{})
        : name_(name), speed_(speed), props_(props), images_(images), sounds_(sounds) {}

    const AssetPaths images() const {
        return images_;
    }

    const AssetPaths sounds() const {
        return sounds_;
    }

    tl::expected<GameObject, ErrorPtr> create_object(AssetManager<sf::Texture>& textures
                                                     /*AssetManager<sf::Sound>& sounds*/) const;

  private:
    std::string name_;
    int speed_;

    Properties props_;

    AssetPaths images_;
    AssetPaths sounds_;

    // collision::Props
    // hitbox options
};
