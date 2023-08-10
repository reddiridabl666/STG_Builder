#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameInfo.hpp"
#include "GameObject.hpp"
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
