#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"

ErrorOr<GameObject> ObjectType::create_object(const ObjectOptions& opts,
                                              AssetManager<sf::Texture>& textures) {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));
    auto obj_name = fmt::format("{}-{}", name, obj_count_);

    GameObject res(obj_name, size, std::move(displayable), speed, tag, props, opts.activity_start,
                   opts.life_func, opts.move);
    opts.set_props(res);

    GameState::get().emit(GameState::Event::ObjectCreated, res.tag());

    // TODO: set hitbox properties
    // set collision rules
    // set bullet rules

    ++obj_count_;
    return res;
}

ErrorOr<GameObject> ObjectType::create_player(AssetManager<sf::Texture>& textures, const GameField& field,
                                              const PlayerOptions& opts) {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));

    GameObject res(fmt::format("player-{}", opts.num), size, std::move(displayable), speed,
                   GameObject::Tag::Player, props, GameObject::kDefaultActivityStart, alive::always,
                   movement::user_control(opts.num, opts.keys, opts.joy));

    res.set_pos(field.center().x, field.bottom() - size.y);

    GameState::get().emit(GameState::Event::ObjectCreated, res.tag());

    return res;
}
