#pragma once

#include "Controls.hpp"
#include "GameObject.hpp"
#include "Text.hpp"

static constexpr const char* kPlayerNum = "__player_num";

struct PlayerOptions {
    size_t num;
    KeyControls keys = kDefaultKeyControls;
    JoyControls joy = kDefaultJoyControls;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerOptions, keys, joy)

struct PlayerMarker {
  public:
    PlayerMarker(Text&& props, const std::shared_ptr<GameObject>& player, const sf::Vector2f& offset);

    void draw(Window& window);

  private:
    Text text_;
    sf::Vector2f offset_;
    std::weak_ptr<GameObject> parent_;
};

struct PlayerMarkerProps {
    TextProps text;
    sf::Vector2f offset = {0, -20};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PlayerMarkerProps, text, offset)

using PlayerList = std::vector<std::shared_ptr<GameObject>>;
