#include "ObjectType.hpp"

#include "AssetManager.hpp"
#include "GameObject.hpp"
#include "SpriteObject.hpp"

ErrorOr<GameObject> ObjectType::create_object(AssetManager<sf::Texture>& textures) {
    auto texture = textures.get(images[0]);
    if (!texture) {
        return tl::unexpected(texture.error());
    }

    auto displayable = std::make_unique<SpriteObject>(std::move(*texture));

    // TODO: managa object tags better
    GameObject res(std::move(displayable), speed, GameObject::Tag::Object, props);
    res.set_size(size);
    res.set_name(fmt::format("{}-{}", name, obj_count));

    // set hitbox properties
    // set collision rules
    // set bullet rules

    ++obj_count;
    return res;
}
