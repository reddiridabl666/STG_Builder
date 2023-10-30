#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <tl/expected.hpp>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "ObjectOptions.hpp"
#include "Properties.hpp"

class Player;
struct PlayerOptions;

class ObjectType {
  public:
    using AssetPaths = std::vector<std::string>;

    ObjectType(const std::string& name = "", const sf::Vector2f& size = {}) : name(name), size(size) {}

    ErrorOr<std::shared_ptr<GameObject>> create_object(const ObjectOptions& opts, assets::Manager& assets);

    ErrorOr<std::shared_ptr<GameObject>> create_player(const ObjectOptions& obj_opts, assets::Manager& assets,
                                                       const PlayerOptions& opts);

    void reset_count() {
        obj_count_ = 0;
    }

    std::string name;
    int speed;
    sf::Vector2f size;

    Properties props;

    GameObject::Tag tag = GameObject::Tag::Object;

    AssetPaths images;
    AssetPaths sounds;

  private:
    size_t obj_count_ = 0;

    ErrorOr<std::shared_ptr<sf::Texture>> get_texture(assets::Textures& textures);
    // collision::Props
    // hitbox options
};
