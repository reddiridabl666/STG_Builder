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

    ObjectType(const std::string& name = "") : name(name) {}

    ErrorOr<GameObject> create_object(AssetManager<sf::Texture>& textures
                                      /*AssetManager<sf::Sound>& sounds*/);

    std::string name;
    int speed;
    sf::Vector2f size;

    Properties props;

    GameObject::Tag tag = GameObject::Tag::Object;

    AssetPaths images;
    AssetPaths sounds;

  private:
    size_t obj_count_ = 0;

    // collision::Props
    // hitbox options
};
