#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "HitboxFactory.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"

std::shared_ptr<GameObject> ObjectType::create_object(const ObjectOptions& opts, assets::Manager& assets) {
    auto texture = get_texture(assets.textures());

    auto displayable = std::make_unique<SpriteObject>(std::move(texture));
    auto obj_name = fmt::format("{}-{}", name, obj_count_);

    auto hitbox = HitboxFactory::create(hitbox_props);

    auto res =
        std::make_shared<GameObject>(obj_name, size, std::move(displayable), speed, tag, props, opts.activity_start,
                                     opts.life_func, opts.move->clone(), std::move(hitbox), opts.stop_at_bounds);
    opts.set_props(*res);

    GameState::get().emit(GameState::Event::ObjectCreated, res->tag());

    // set collision rules
    // set bullet rules

    ++obj_count_;
    return res;
}

std::shared_ptr<GameObject> ObjectType::create_player(const ObjectOptions& obj_opts, assets::Manager& assets,
                                                      const PlayerOptions& opts) {
    auto player = create_object(obj_opts, assets);

    player->set_life_update(alive::always);
    player->set_movement(movement::user_control(opts.num, opts.keys, opts.joy));
    player->props().set(kPlayerNum, opts.num);

    return player;
}

std::shared_ptr<sf::Texture> ObjectType::get_texture(assets::Textures& textures) {
    if (images.empty()) {
        auto texture = textures.get(assets::kFallbackImage);
        if (!texture) {
            throw texture.error();
        }
    }

    return textures.get_or(images[0], assets::kFallbackImage);
}
