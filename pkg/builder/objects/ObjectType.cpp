#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "Player.hpp"
#include "SpriteObject.hpp"

ErrorOr<GameObject> ObjectType::create_object(AssetManager<sf::Texture>& textures) {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));
    auto obj_name = fmt::format("{}-{}", name, obj_count_);

    GameObject res(obj_name, size, std::move(displayable), speed, tag, props);

    Game::info().emit(Game::Event::ObjectCreated, res.tag());

    // TODO: set hitbox properties
    // set collision rules
    // set bullet rules

    ++obj_count_;
    return res;
}

// ErrorOr<std::shared_ptr<Player>> ObjectType::create_player(AssetManager<sf::Texture>& textures,
//                                                            int player_num) {
//     auto texture = textures.get(images[0]);
//     if (!texture) {
//         return tl::unexpected(texture.error());
//     }

//     auto displayable = std::make_unique<SpriteObject>(std::move(*texture));

//     auto res = std::make_shared<Player>(size, std::move(displayable), speed, props, player_num);

//     Game::info().emit(Game::Event::ObjectCreated, res->tag());

//     ++obj_count_;
//     return res;
// }

ErrorOr<GameObject> ObjectType::create_player(AssetManager<sf::Texture>& textures, const GameField& field,
                                              int player_num) {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));

    GameObject res(fmt::format("player-{}", player_num), size, std::move(displayable), speed,
                   GameObject::Tag::Player, props);

    // TODO: ugly
    res.set_movement(movement::user_control(player_num));
    res.set_life_update(alive::always);
    res.set_pos(field.center().x, field.bottom() - size.y);

    Game::info().emit(Game::Event::ObjectCreated, res.tag());

    ++obj_count_;
    return res;
}
