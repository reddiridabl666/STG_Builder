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

class ObjectType {
  public:
    using AssetPaths = std::vector<std::string>;

    // ObjectType(const std::string& name, const Properties::Data& props)
    //     : Properties(props), name_(name) {}
    ObjectType(const std::string& name = "") : name(name) {}

    ObjectType(const std::string& name, int speed, sf::Vector2f size, const Properties& props,
               const Asset& image)
        : name(name), speed(speed), size(size), props(props), images{image.path} {}

    ObjectType(const std::string& name, int speed, sf::Vector2f size, const Properties& props,
               const AssetPaths& images, const AssetPaths& sounds = AssetPaths{})
        : name(name), speed(speed), size(size), props(props), images(images), sounds(sounds) {}

    ErrorOr<GameObject> create_object(AssetManager<sf::Texture>& textures
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
