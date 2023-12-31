#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameState.hpp"
#include "HitboxFactory.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"
#include "actions/ActionFactory.hpp"
#include "actions/ActionMap.hpp"

std::shared_ptr<GameObject> ObjectType::create_object(const ObjectOptions& opts, assets::Manager& assets) const {
    auto texture = get_texture(assets.textures());

    auto displayable = std::make_unique<SpriteObject>(std::move(texture));
    auto obj_name = fmt::format("{}-{}", name, obj_count_);

    auto hitbox = HitboxFactory::create(hitbox_props);

    // clang-format off
    auto res = std::make_shared<GameObject>(
        obj_name,
        size,
        std::move(displayable),
        speed,
        tag,
        props,
        opts.activity_start,
        opts.life_func,
        opts.move->clone(), 
        std::move(hitbox),
        timed_actions.is_null() ? std::vector<action::Timed>{} : timed_actions.get<std::vector<action::Timed>>(),
        // collision.get<GameObject::CollisionAction>(),
        // on_player.get<GameObject::PlayerAction>(),
        // on_own.get<GameObject::OwnAction>(),
        // action::Factory::create<action::BinaryAction>(on_death),
        opts.stop_at_bounds
    );
    // clang-format on

    res->set_life_update(life_func);

    opts.set_props(*res);

    GameState::get().emit(GameState::Event::ObjectCreated, *res);

    ++obj_count_;
    return res;
}

std::shared_ptr<GameObject> ObjectType::create_player(const ObjectOptions& obj_opts, assets::Manager& assets,
                                                      const PlayerOptions& opts) const {
    auto player = create_object(obj_opts, assets);

    player->set_movement(movement::user_control(opts.num, opts.keys, opts.joy));
    player->props().set(kPlayerNum, opts.num);

    return player;
}

std::shared_ptr<sf::Texture> ObjectType::get_texture(assets::Textures& textures) const {
    if (images.empty()) {
        auto texture = textures.get(assets::kFallbackImage);
        if (!texture) {
            throw texture.error();
        }
    }

    return textures.get_or(images[0], assets::kFallbackImage);
}

Pattern* ObjectType::pattern(const std::string& name) {
    auto pattern = patterns.find(name);
    if (pattern == patterns.end()) {
        return nullptr;
    }
    return pattern->second.get();
}
