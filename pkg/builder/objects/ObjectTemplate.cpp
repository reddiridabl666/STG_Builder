#include "ObjectTemplate.hpp"

#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "SpriteObject.hpp"

tl::expected<GameObject, ErrorPtr> ObjectTemplate::create_object(AssetManager<sf::Texture>& textures) const {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));

    GameObject res(std::move(displayable), speed, props);
    res.set_size(size);

    // set hitbox properties
    // set collision rules
    // set bullet rules

    return res;
}
