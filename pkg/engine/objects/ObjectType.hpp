#pragma once

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Asset.hpp"
#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "HitboxProps.hpp"
#include "ObjectOptions.hpp"
#include "Pattern.hpp"
#include "Properties.hpp"

class Player;
struct PlayerOptions;

static constexpr const char* kPlayerNum = "__player_num";

class ObjectType {
  public:
    using AssetPaths = std::vector<std::string>;

    ObjectType(const std::string& name = "", const sf::Vector2f& size = {}) : name(name), size(size) {}

    std::shared_ptr<GameObject> create_object(const ObjectOptions& opts, assets::Manager& assets);

    std::shared_ptr<GameObject> create_player(const ObjectOptions& obj_opts, assets::Manager& assets,
                                              const PlayerOptions& opts);

    void reset_count() {
        obj_count_ = 0;
    }

    Pattern* pattern(const std::string&);

    std::string name;
    Value speed;
    sf::Vector2f size;

    Properties props;

    GameObjectTag tag = GameObjectTag::Object;

    AssetPaths images;
    AssetPaths sounds;

    HitboxProps hitbox_props;

    nl::json collision;
    nl::json on_death;
    nl::json on_player;
    nl::json on_own;

    std::unordered_map<std::string, std::unique_ptr<Pattern>> patterns;

  private:
    size_t obj_count_ = 0;

    std::shared_ptr<sf::Texture> get_texture(assets::Textures& textures);
};
