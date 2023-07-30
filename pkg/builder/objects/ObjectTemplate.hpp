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
    ObjectTemplate(const std::string& name = "") : name(name) {}

    ObjectTemplate(const std::string& name, int speed, sf::Vector2f size, const Properties& props,
                   const Asset& image)
        : name(name), speed(speed), size(size), props(props), images{image.path} {}

    ObjectTemplate(const std::string& name, int speed, sf::Vector2f size, const Properties& props,
                   const AssetPaths& images, const AssetPaths& sounds = AssetPaths{})
        : name(name), speed(speed), size(size), props(props), images(images), sounds(sounds) {}

    tl::expected<GameObject, ErrorPtr> create_object(AssetManager<sf::Texture>& textures
                                                     /*AssetManager<sf::Sound>& sounds*/) const;

    std::string name;
    int speed;
    sf::Vector2f size;

    Properties props;

    AssetPaths images;
    AssetPaths sounds;

    // collision::Props
    // hitbox options
};
