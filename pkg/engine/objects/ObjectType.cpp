#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"

ErrorOr<std::shared_ptr<GameObject>> ObjectType::create_object(const ObjectOptions& opts, assets::Manager& assets) {
    auto texture = get_texture(assets.textures());
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));
    auto obj_name = fmt::format("{}-{}", name, obj_count_);

    auto res = std::make_shared<GameObject>(obj_name, size, std::move(displayable), speed, tag, props,
                                            opts.activity_start, opts.life_func, opts.move->clone());
    opts.set_props(*res);

    GameState::get().emit(GameState::Event::ObjectCreated, res->tag());

    // TODO: set hitbox properties
    // set collision rules
    // set bullet rules

    ++obj_count_;
    return res;
}

ErrorOr<std::shared_ptr<GameObject>> ObjectType::create_player(const ObjectOptions& obj_opts, assets::Manager& assets,
                                                               const PlayerOptions& opts) {
    auto player = create_object(obj_opts, assets);
    if (!player) {
        return player;
    }

    (*player)->set_life_update(alive::always);
    (*player)->set_movement(movement::user_control(opts.num, opts.keys, opts.joy));
    (*player)->props().set(kPlayerNum, opts.num);

    return player;
}

ErrorOr<std::shared_ptr<sf::Texture>> ObjectType::get_texture(assets::Textures& textures) {
    if (images.empty()) {
        return textures.get(assets::kFallbackImage);
    }

    std::shared_ptr<sf::Texture> texture;
    try {
        texture = textures.get_or(images[0], assets::kFallbackImage);
    } catch (std::exception& e) {
        return Error::New(e.what());
    }

    return texture;
}
